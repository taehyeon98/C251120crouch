// Fill out your copyright notice in the Description page of Project Settings.


#include "../Title/TitlePC.h"
#include "TitleWidget.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	//경로 설정
	FSoftClassPath TitleWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyProject/Blueprint/Title/WBP_Title.WBP_Title_C'"));

	//include "path" ---> class 정의
	UClass* WidgetClass = TitleWidgetClass.TryLoadClass<UTitleWidget>();

	if (WidgetClass)
	{
		//new Class
		TitleWidgetObject = CreateWidget<UTitleWidget>(this, WidgetClass);
		if (TitleWidgetObject)
		{
			TitleWidgetObject->AddToViewport();
		}
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void ATitlePC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}
