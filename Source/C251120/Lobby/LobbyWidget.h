// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"


class UButton;
class UEditableTextBox;
class UTextBlock;
class UScrollBox;

/**
 * 
 */
UCLASS()
class C251120_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void Start();

	UFUNCTION()
	void ProcessOnCommite(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void ProcessOnChange(const FText& Text);

	UFUNCTION()
	void UpdateLeftTime(int32 inLeftTime);

	UFUNCTION()
	void UpdateConnectPlayer(int32 InConnectCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Component, meta=(WidgetBind))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Component, meta=(WidgetBind))
	TObjectPtr<UEditableTextBox> ChatInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Component, meta=(WidgetBind))
	TObjectPtr<UTextBlock> ConnectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Component, meta=(WidgetBind))
	TObjectPtr<UTextBlock> LeftTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Component, meta=(WidgetBind))
	TObjectPtr<UScrollBox> ChatScrollBox;


	void AddMessage(const FText& Message);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data)
	TObjectPtr<UDataTable> ChatStyleSet;

	void ShowStartButton();
};
