// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CChar.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 10 UMETA(DisplayName = "Pistol"),
	Rifle = 20 UMETA(DisplayName = "Rifle")
};

UCLASS()
class C251120_API ACChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool CanCrouch() const override;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category = Component)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Component)
	TObjectPtr<class UCameraComponent> Camera;

	UFUNCTION(BlueprintCallable)
	void Move(float Forward, float Right);

	UFUNCTION(BlueprintCallable)
	void Aim(float Pitch, float Yaw);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bLeftLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bRightLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bCrouch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EWeaponState WeaponState = EWeaponState::Unarmed;
};
