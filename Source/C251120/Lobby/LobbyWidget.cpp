// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameStateBase.h"
#include "LobbyPC.h"
#include "../Title/TitleGameInstanceSubsystem.h"
#include "Components/RichTextBlock.h"

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::Start);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ProcessOnCommite);
		ChatInput->OnTextChanged.AddDynamic(this, &ULobbyWidget::ProcessOnChange);
	}

	ALobbyGameStateBase* GS = Cast<ALobbyGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangeLeftTime.AddDynamic(this, &ULobbyWidget::UpdateLeftTime);
		GS->OnCountingConnectPlayer.AddDynamic(this, &ULobbyWidget::UpdateConnectPlayer);
	}
}

void ULobbyWidget::Start()
{
	GetWorld()->ServerTravel(TEXT("InGame"));
}

void ULobbyWidget::ProcessOnCommite(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer());
			if (PC)
			{
				UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
				if (GI)
				{
					UTitleGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<UTitleGameInstanceSubsystem>();
					//MySubsystem->UserID = 
					FString Temp = FString::Printf(TEXT("%s : %s"), *MySubsystem->UserID, *Text.ToString());

					//Local PC Call -> Server PC execute
					PC->C2S_SendMessage(FText::FromString(Temp));
					ChatInput->SetText(FText::FromString(TEXT("")));
				}
			}
		}
		break;

		case ETextCommit::OnCleared:
		{
			ChatInput->SetUserFocus(GetOwningPlayer());
		}
		break;
	}
}

void ULobbyWidget::ProcessOnChange(const FText& Text)
{

}

void ULobbyWidget::UpdateLeftTime(int32 inLeftTime)
{
	if (LeftTime)
	{
		FString Message = FString::Printf(TEXT("%d초 남음"), inLeftTime);
		LeftTime->SetText(FText::FromString(Message));
		if (inLeftTime == 0)
		{
			Start();
		}
	}
}

void ULobbyWidget::UpdateConnectPlayer(int32 InConnectCount)
{
	if (ConnectionCount)
	{
		FString Count = FString::Printf(TEXT("%d명 접속"), InConnectCount);
		ConnectionCount->SetText(FText::FromString(Count));
	}
}

void ULobbyWidget::AddMessage(const FText& Message)
{
	if (ChatScrollBox)
	{
		UTextBlock* NewMessageBlock = NewObject<UTextBlock>(ChatScrollBox);
		if (NewMessageBlock)
		{
			NewMessageBlock->SetText(Message);
			FSlateFontInfo FontInfo = NewMessageBlock->GetFont();
			FontInfo.Size = 25;
			NewMessageBlock->SetFont(FontInfo);
			NewMessageBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0, 0, 1)));
			
			ChatScrollBox->AddChild(NewMessageBlock);
			ChatScrollBox->ScrollToEnd();
		}
	}
}

void ULobbyWidget::ShowStartButton()
{
	if (StartButton)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
}
