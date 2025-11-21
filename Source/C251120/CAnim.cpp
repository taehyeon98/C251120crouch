// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnim.h"
#include "CChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UCAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ACChar* Char = Cast<ACChar>(TryGetPawnOwner());
	if (Char)
	{
		GroundSpeed = Char->GetCharacterMovement()->Velocity.Size2D();
		Direction = UKismetAnimationLibrary::CalculateDirection(Char->GetCharacterMovement()->Velocity, Char->GetActorRotation());
		AimYaw = Char->GetBaseAimRotation().Yaw;
		AimPitch = Char->GetBaseAimRotation().Pitch;
		bAiming = Char->bAiming;
		bLeftLean = Char->bLeftLean;
		bRightLean = Char->bRightLean;
		bIsCrouched = Char->bIsCrouched;

		float TargetLeanAngle = 0;
		if (bLeftLean)
		{
			TargetLeanAngle = -30.0f;
		}
		else if (bRightLean)
		{
			TargetLeanAngle = 30.0f;
		}
		else
		{
			TargetLeanAngle = 0;
		}

		CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetLeanAngle, DeltaSeconds, 8.0f);

		WeaponState = Char->WeaponState;
	}

}

void UCAnim::AnimNotify_SkeletonNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_SkeletonNotify"));
}
