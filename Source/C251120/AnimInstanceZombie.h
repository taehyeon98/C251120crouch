// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPPZombie.h"
#include "AnimInstanceZombie.generated.h"

/**
 * 
 */
UCLASS()
class C251120_API UAnimInstanceZombie : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EZombieState CurrentState;
};
