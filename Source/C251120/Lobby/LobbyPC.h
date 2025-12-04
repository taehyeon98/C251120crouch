// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"


class ULobbyWidget;
/**
 * 
 */
UCLASS()
class C251120_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPC();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	//자동으로 네트워크를 통해서 실행하는 코드 생성
	//이 코드는 네트워크 던지는 코드 생성.
	//Reliable = 네트워크로 던져놓고 확인까지 한다는 의미.
	//WithValidation = 값이 맞는지 확인하는것.
	UFUNCTION(Server,Reliable, WithValidation)
	void C2S_SendMessage(const FText& Message);
	bool C2S_SendMessage_Validate(const FText& Message);

	//구현은 이 함수로 구현.
	//실제 동작
	//문자를 보내야 할때는 Const,& 가 필요하다.(객체를 보낼때도)
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client,Reliable)
	void S2C_SendMessage(const FText& Message);//network 전보 던져서 실행, 자동생성.
	void S2C_SendMessage_Implementation(const FText& Message);//구현

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data)
	TObjectPtr<ULobbyWidget> LobbyWidgetObject;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data)
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;
};
