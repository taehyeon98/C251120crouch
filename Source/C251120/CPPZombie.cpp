// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPZombie.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "ZombieController.h"

// Sets default values
ACPPZombie::ACPPZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACPPZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPPZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*APawn* TargetPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	FVector Direction = TargetPawn->GetActorLocation() - GetActorLocation();
	AddMovementInput(Direction, 1);

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPawn->GetActorLocation());
	SetActorRotation(Rotation);*/
}

void ACPPZombie::SetState(EZombieState NewState)
{
	CurrentState = NewState;
}

float ACPPZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (CurrentHP <= 0)
	{
		SetState(EZombieState::Death);

		return DamageAmount;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
		}

		SpawnHitEffect(Event->HitInfo);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;

			UE_LOG(LogTemp, Warning, TEXT("Radial Damage %f %s"), DamageAmount, *Event->DamageTypeClass->GetName());
		}
	}
	else //(DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}



	if (CurrentHP <= 0)
	{
		//죽는다. 애님 몽타주 재생
		//네트워크 할려면 다 RPC로 작업해 됨
		DoDead();
		SetState(EZombieState::Death);
		AZombieController* AIC = Cast<AZombieController>(GetController());
		if (AIC)
		{
			AIC->SetState(EZombieState::Death);
		}
	}

	return DamageAmount;
}

void ACPPZombie::DoDeadEnd()
{
	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ACPPZombie::DoDead()
{
	FName SectionName = FName(FString::Printf(TEXT("%d"), FMath::RandRange(1, 6)));
}

void ACPPZombie::SpawnHitEffect(FHitResult Hit)
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

void ACPPZombie::ChangeSpeed(float NewMaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}

