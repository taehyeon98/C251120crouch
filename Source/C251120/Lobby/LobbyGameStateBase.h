// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ObjectMacros.h"
#include "LobbyGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedLeftTime, const int32, InLeftTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountingConnectPlayer, const int32, InConnectionCount);

/**
 * 
 */
UCLASS()
class C251120_API ALobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_LeftTime();

	UFUNCTION()
	void OnRep_ConnectCount();

	virtual void BeginPlay() override;

	//네트워크시에는 Replicated가 필요하다. 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data , ReplicatedUsing="OnRep_LeftTime")
	int32 LeftTime = 60;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data , ReplicatedUsing = "OnRep_ConnectCount")
	int32 ConnectionCount = 0;

	FOnCountingConnectPlayer OnCountingConnectPlayer;

	FOnChangedLeftTime OnChangeLeftTime;
};
