// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasePCM.generated.h"

/**
 * 
 */
UCLASS()
class C251120_API ABasePCM : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ABasePCM();

	virtual void UpdateCamera(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float NormalFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float IronSightFOV = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float ZoomSpeed = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CurrentZoom;
};
