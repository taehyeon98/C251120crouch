// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include "CChar.generated.h"
class UInputAction;
class APickupItem;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 10 UMETA(DisplayName = "Pistol"),
	Rifle = 20 UMETA(DisplayName = "Rifle"),
	Launcher = 30 UMETA(DisplayName = "Launcher")
};

UCLASS()
class C251120_API ACChar : public ACharacter, public IGenericTeamAgentInterface
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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void ProcessActorBeginOverlap( AActor* OverlappedActor, AActor* OtherActor );

	void EatItem(APickupItem* PickedupItem);

	void UseItem(APickupItem* PickedupItem);

	void EquipItem(APickupItem* PickedupItem);

	void StopIronSight();

	void StartIronSight();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category = Component)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Component)
	TObjectPtr<class UCameraComponent> Camera;

	UFUNCTION(BlueprintCallable)
	void Move(float Forward, float Right);

	UFUNCTION(BlueprintCallable)
	void Aim(float Pitch, float Yaw);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<UMaterialInstance> Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bLeftLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bRightLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UParticleSystem> BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_IronSight;


	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UChildActorComponent> Weapon;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnHitEffect(FHitResult Hit);

	UFUNCTION(BlueprintCallable)
	void HitReaction();

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void DeathReaction();

	UFUNCTION(BlueprintCallable)
	void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsFire : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsIronSight : 1 = false;

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void DoFire();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Character)
	FGenericTeamId TeamID;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;
};
