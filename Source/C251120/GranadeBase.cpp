// Fill out your copyright notice in the Description page of Project Settings.


#include "GranadeBase.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "Engine/DamageEvents.h"
#include "BaseDamageType.h"

void AGranadeBase::Fire()
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
			EDrawDebugTrace::ForDuration,
			HitResult,
			true
		);
		if (bResult)
		{
			//HitResult.GetActor();
			//RPG
			/*UGameplayStatics::ApplyDamage(
				HitResult.GetActor(),
				50,
				GetController(),
				this,
				UBaseDamageType::StaticClass()
			);
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());*/

			//총쏘는 데미지
		/*	UGameplayStatics::ApplyPointDamage(
				HitResult.GetActor(),
				10,
				-HitResult.ImpactNormal,
				HitResult,
				PC,
				this,
				UBaseDamageType::StaticClass()
			);*/
			//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());

			//범위공격, 폭탄
			UGameplayStatics::ApplyRadialDamage(
				HitResult.GetActor(), 
				50, 
				HitResult.ImpactPoint,
				300.0f, 
				UBaseDamageType::StaticClass(),
				IngnoreActors,
				this,
				PC,
				true
			);
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());


		}
	}
	CurrentBulletCount--;
	UE_LOG(LogTemp, Warning, TEXT("Fire %d"), CurrentBulletCount);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PistolSound, GetActorLocation());
	TimeofLastShoot = GetWorld()->TimeSeconds;
}
