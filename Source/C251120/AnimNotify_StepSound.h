// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StepSound.generated.h"

/**
 * 
 */
UCLASS()
class C251120_API UAnimNotify_StepSound : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	//만들때 반드시 구현
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
