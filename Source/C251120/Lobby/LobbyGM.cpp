// Fill out your copyright notice in the Description page of Project Settings.


#include "../Lobby/LobbyGM.h"
#include "LobbyGameStateBase.h"
#include "LobbyWidget.h"

ALobbyGM::ALobbyGM()
{
}

void ALobbyGM::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//ErrorMessage에 메시지 쓰면 튕긴다.
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Options);
}

APlayerController* ALobbyGM::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	return PC;
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);	
	CheckConnectCount();
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		LeftTimerHandle,
		FTimerDelegate::CreateLambda([this]() {
			ALobbyGameStateBase* GS = GetGameState<ALobbyGameStateBase>();
			if (GS)
			{
				//Server
				if (GS->LeftTime != 0)
				{
					GS->LeftTime--;
					GS->OnRep_LeftTime();
				}
			}
		}),
		1.0f,
		true,
		0.0f
	);

	//BeginPlay()보다 UI가 느려서 업데이트 한번함.
	CheckConnectCount();
}

void ALobbyGM::CheckConnectCount()
{
	ALobbyGameStateBase* GS = GetGameState<ALobbyGameStateBase>();
	if (GS)
	{
		//Client다 가능한데 서버에서는 업데이트가 안됨.
		int32 TempCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
		{
			TempCount++;
		}

		GS->ConnectionCount = TempCount;
		GS->OnRep_ConnectCount();
	}
}
