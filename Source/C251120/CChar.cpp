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

}

// Called when the game starts or when spawned
void ACChar::BeginPlay()
{
	Super::BeginPlay();

	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
		WeaponState = EWeaponState::Pistol;
		ChildWeapon->SetOwner(this);
	}
	
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

			HitReaction();
		}
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
	
	if (CurrentHP <= 0)
	{
		//네트워크 할려면 다 RPC로 작업.
		DeathReaction();
	}

	return 0.0f;
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
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	
}



