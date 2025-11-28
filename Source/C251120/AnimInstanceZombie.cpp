// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimInstanceZombie::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACPPZombie* Zombie = Cast<ACPPZombie>(TryGetPawnOwner());
	if (Zombie)
	{
		Speed = Zombie->GetCharacterMovement()->Velocity.Size2D();
		CurrentState = Zombie->CurrentState;
	}
}
