// Fill out your copyright notice in the Description page of Project Settings.


#include "../Network/NetworkUtil.h"

NetworkUtil::NetworkUtil()
{
}

NetworkUtil::~NetworkUtil()
{
}

void NetworkUtil::Log(UWorld* World,const FString& InString)
{
	if (!World)
	{
		return;
	}
	FString Prefix;
	switch (World->GetNetMode())
	{
	case NM_Client:
		Prefix = FString::Printf(TEXT("Client %d: "), UE::GetPlayInEditorID());
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		Prefix = FString::Printf(TEXT("Server: "));
		break;
	case NM_Standalone:
		break;
	}
}
