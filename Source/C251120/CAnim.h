// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CChar.h"
#include "CAnim.generated.h"

/**
 * 
 */
UCLASS()
class C251120_API UCAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = Speed)
	float GroundSpeed = 0;

	UFUNCTION()
	void AnimNotify_SkeletonNotify();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Speed)
	float Direction = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bLeftLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bRightLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float CurrentLeanAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bCrouch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EWeaponState WeaponState = EWeaponState::Unarmed;
};
