// Fill out your copyright notice in the Description page of Project Settings.


#include "../Lobby/LobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPC.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyWidget.h"


void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//변수를 replicated등록해주는 매크로
	//서버에서 GameState값이 변하게 되면 클라이언트에 복제해준다.
	//클라이언트에서 서버로는 X.
	DOREPLIFETIME(ALobbyGameStateBase, LeftTime);
	DOREPLIFETIME(ALobbyGameStateBase, ConnectionCount);
}

//값바뀔때 호출하기
void ALobbyGameStateBase::OnRep_LeftTime()
{
	/*ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (PC && PC->LobbyWidgetObject)
	{
		PC->LobbyWidgetObject->UpdateLeftTime(LeftTime);
	}*/

	OnChangeLeftTime.Broadcast(LeftTime);
}

void ALobbyGameStateBase::OnRep_ConnectCount()
{
	OnCountingConnectPlayer.Broadcast(ConnectionCount);
}
void ALobbyGameStateBase::BeginPlay()
{
}
