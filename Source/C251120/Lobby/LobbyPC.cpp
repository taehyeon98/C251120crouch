// Fill out your copyright notice in the Description page of Project Settings.


#include "../Lobby/LobbyPC.h"
#include "LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameStateBase.h"
#include "LobbyGM.h"

ALobbyPC::ALobbyPC()
{
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass)
	{
		//UI만들땐 PC에 만들기.
		//꼭 LocalPC인지 확인해야함.
		if (IsLocalPlayerController())
		{
			LobbyWidgetObject = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
			LobbyWidgetObject->AddToViewport();

			if (LobbyWidgetObject && HasAuthority())
			{
				LobbyWidgetObject->ShowStartButton();
			}

			//Listen Server일때만 실행.
			ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->CheckConnectCount();
			}
		}
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void ALobbyPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
}

bool ALobbyPC::C2S_SendMessage_Validate(const FText& Message)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FText& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			PC->S2C_SendMessage(Message);
		}
	}
}

void ALobbyPC::S2C_SendMessage_Implementation(const FText& Message)
{
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->AddMessage(Message);
	}
}
