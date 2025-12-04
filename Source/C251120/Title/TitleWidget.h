// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

class UButton;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class C251120_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Widget, meta=(WidgetBind))
	TObjectPtr<UButton> StartServerButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (WidgetBind))
	TObjectPtr<UButton> ConnectButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> UserID;

	//meta=(WidgetBind) = 이름 같은것을 알아서 찾아준다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<UEditableTextBox> ServerIP;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartServer();

	UFUNCTION()
	void Connect();

	UFUNCTION()
	void SaveData();
};
