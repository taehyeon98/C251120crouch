// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TitleGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class C251120_API UTitleGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString UserID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString Password;
};
