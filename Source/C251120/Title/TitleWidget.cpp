// Fill out your copyright notice in the Description page of Project Settings.


#include "../Title/TitleWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TitleGameInstanceSubsystem.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//같은이름을 찾아서 리턴해줌.
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));
	
	StartServerButton->OnClicked.AddDynamic(this, &UTitleWidget::StartServer);
	ConnectButton->OnClicked.AddDynamic(this, &UTitleWidget::Connect);
}

void UTitleWidget::StartServer()
{
	SaveData();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UTitleWidget::Connect()
{
	SaveData();
	UGameplayStatics::OpenLevel(GetWorld(), FName(ServerIP->GetText().ToString()),true, TEXT("Options"));
}

void UTitleWidget::SaveData()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if(GI)
	{
		UTitleGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<UTitleGameInstanceSubsystem>();
		MySubsystem->UserID = UserID->GetText().ToString();
		MySubsystem->Password = Password->GetText().ToString();
	}
}
