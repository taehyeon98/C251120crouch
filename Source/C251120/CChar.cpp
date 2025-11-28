// Fill out your copyright notice in the Description page of Project Settings.


#include "CChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Components/ChildActorComponent.h"
#include "WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BaseDamageType.h"
#include "Engine/DamageEvents.h"
#include "Components/PrimitiveComponent.h"
#include "PickupItem.h"
#include "Components/DecalComponent.h"
#include "CPPZombie.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>

// Sets default values
ACChar::ACChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->AddRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->AddRelativeRotation(FRotator(0, -90.0f, 0));

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	TeamID = 1;
}

// Called when the game starts or when spawned
void ACChar::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ACChar::ProcessActorBeginOverlap);
}

// Called every frame
void ACChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Reload, ETriggerEvent::Completed, this,
			&ACChar::Reload);
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this,
			&ACChar::StopFire);
		UIC->BindAction(IA_Fire, ETriggerEvent::Started, this,
			&ACChar::StartFire);
		UIC->BindAction(IA_IronSight, ETriggerEvent::Started, this,
			&ACChar::StartIronSight);
		UIC->BindAction(IA_IronSight, ETriggerEvent::Completed, this,
			&ACChar::StopIronSight);
	}
}

bool ACChar::CanCrouch() const
{
	return true;
}

float ACChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return DamageAmount;
	}
	//
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;

			UE_LOG(LogTemp, Warning, TEXT("PointDamage %f %s"), DamageAmount ,*(Event->HitInfo.BoneName.ToString()));

		}
		
		SpawnHitEffect(Event->HitInfo);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
			UE_LOG(LogTemp, Warning, TEXT("RadialDamage %f"), DamageAmount);
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}
	
	HitReaction();
	if (CurrentHP <= 0)
	{
		//네트워크 할려면 다 RPC로 작업.
		DeathReaction();
	}

	return DamageAmount;
}

void ACChar::Move(float Forward, float Right)
{
	const FRotator CharRotation = FRotator(GetController()->GetControlRotation());
	const FRotator CharRotationYaw= FRotator(0, CharRotation.Yaw, 0);
	
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CharRotationYaw);
	AddMovementInput(ForwardVector, Forward);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(CharRotationYaw);
	AddMovementInput(RightVector, Right);
}

void ACChar::Aim(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

void ACChar::Reload()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		PlayAnimMontage(ChildWeapon->ReloadMontage);
	}
}

void ACChar::DoFire()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Fire();
	}
}

void ACChar::StartFire()
{
	bIsFire = true;
	DoFire();
}

void ACChar::StopFire()
{
	bIsFire = false;
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->StopFire();
	}
}

void ACChar::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ACChar::GetGenericTeamId() const
{
	return TeamID;
}


void ACChar::HitReaction()
{
	FString SectionName = FString::Printf(TEXT("%d"), FMath::RandRange(1, 8));

	PlayAnimMontage(HitMontage, 1.0, FName(*SectionName));
}

void ACChar::ReloadWeapon()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Reload();
	}
}

void ACChar::DeathReaction()
{
	FString SectionName = FString::Printf(TEXT("%d"), FMath::RandRange(1, 6));
	PlayAnimMontage(DeathMontage, 1.0, FName(*SectionName));
}


void ACChar::Death()
{
	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ACChar::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	APickupItem* PickedupItem = Cast<APickupItem>(OtherActor);
	if(PickedupItem)
	{
		switch (PickedupItem->ItemType)
		{
		case EItemType::Use:
			UseItem(PickedupItem);
			break;
		case EItemType::Eat:
			EatItem(PickedupItem);
			break;
		case EItemType::Equip:
			EquipItem(PickedupItem);
			break;
		}

		if (!PickedupItem->bIsInfinity)
		{
			PickedupItem->Destroy();
		}
	}
}

void ACChar::EatItem(APickupItem* PickedupItem)
{

}

void ACChar::UseItem(APickupItem* PickedupItem)
{

}

void ACChar::EquipItem(APickupItem* PickedupItem)
{
	Weapon->SetChildActorClass(PickedupItem->ItemTemplate);
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		if (ChildWeapon->Name.Compare(TEXT("Pistol")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Pistol;
			ChildWeapon->SetOwner(this);
		}
		if (ChildWeapon->Name.Compare(TEXT("Rifle")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
		}
		if (ChildWeapon->Name.Compare(TEXT("launcher")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Launcher;
			ChildWeapon->SetOwner(this);
		}
		
	}
}

void ACChar::StartIronSight()
{
	bIsIronSight = true;
}

void ACChar::StopIronSight()
{
	bIsIronSight = false;
}

void ACChar::SpawnHitEffect(FHitResult Hit)
{
	if (BloodEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("BloodEffect"));
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
}


//void ACChar::GetItem()
//{
//
//	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
//	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
//	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
//
//	TArray<AActor*> IngnoreActors;
//	FHitResult HitResult;
//
//	FVector Sphere = GetMesh()->GetComponentLocation();
//	
//	bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(
//		GetWorld(),
//		Sphere,
//		Sphere,
//		60.0f,
//		ObjectTypes,
//		true,
//		IngnoreActors,
//		EDrawDebugTrace::ForDuration,
//		HitResult,
//		true
//	);
//	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());
//
//	if (bResult)
//	{
//		AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
//		if (ChildWeapon)
//		{
//			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
//			FString Item = HitResult.GetActor()->GetName();
//			Item.Split(TEXT("_C"), &Item, nullptr);
//			if (Item == TEXT("BP_GroundPistol"))
//			{
//				WeaponState = EWeaponState::Pistol;
//			}
//			else if (Item == TEXT("BP_GroundRifle"))
//			{
//				WeaponState = EWeaponState::Rifle;
//			}
//			else if (Item == TEXT("BP_GroundLauncher"))
//			{
//				WeaponState = EWeaponState::Launcher;
//			}
//			ChildWeapon->SetOwner(this);
//		}
//	}
//
//}



