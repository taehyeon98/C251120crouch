// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "CChar.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "BaseDamageType.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "ProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Fire()
{

	float CurrentTimeofShoot = GetWorld()->TimeSeconds - TimeofLastShoot;

	//순서 중요함.
	if (CurrentTimeofShoot < RefireRate)
	{
		return;
	}

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(
			RefireTimer,
			this,
			&AWeaponBase::Fire,
			RefireRate,
			false
		);
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	//캐릭터값이 널인지 확인.
	ensure(Character);
	//엔진 꺼질때 확인가능.
	//check(Character);
	if (!Character)
	{
		return;
	}
	




	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (PC)
	{
		int32 SizeX = 0;
		int32 SizeY = 0;
		int32 CenterX = 0;
		int32 CenterY = 0;
		//다이렉션 = 방향.
		FVector WorldDirection;
		//포지션 = 3차원에 있는 물체의 위치
		FVector WorldLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		PC->GetViewportSize(SizeX, SizeY);
		CenterX = SizeX / 2;
		CenterY = SizeY / 2;


		//PC->DeprojectMousePositionToWorld();
		PC->DeprojectScreenPositionToWorld((float)CenterX, (float)CenterY, WorldLocation, WorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = CameraLocation + WorldDirection * 100000.0f;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> IngnoreActors;
		FHitResult HitResult;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			ObjectTypes,
			true,
			IngnoreActors,
			EDrawDebugTrace::None,
			HitResult,
			true
		);

		FVector SpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
		FVector TargetLocation = bResult ? HitResult.ImpactPoint : End;
		FVector BulletDirection = (TargetLocation - SpawnLocation).GetSafeNormal();

		FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));

		
		FireProjectile(FTransform (AimRotation, SpawnLocation, FVector::OneVector), HitResult);

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MuzzleFlash,
			SpawnLocation,
			AimRotation
		);

		//recoil
		/*Character->AddControllerPitchInput(-0.4f);*/
	}
	CurrentBulletCount--;
	UE_LOG(LogTemp, Warning, TEXT("Fire %d"), CurrentBulletCount);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PistolSound, GetActorLocation());
	TimeofLastShoot = GetWorld()->TimeSeconds;
}

void AWeaponBase::FireProjectile(FTransform Transform, FHitResult InHitResult)
{
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileTemplate, Transform);
	Projectile->HitResult = InHitResult;
	Projectile->SetOwner(this);
}

void AWeaponBase::Reload()
{
	CurrentBulletCount = MaxBulletCount;
	UE_LOG(LogTemp, Warning, TEXT("Reload %d"), CurrentBulletCount);
}

void AWeaponBase::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}
