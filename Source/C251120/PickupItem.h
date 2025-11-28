// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Use = 0 UMETA(DisplayName = "Use"),
	Eat = 10 UMETA(DisplayName = "Eat"),
	Equip = 20 UMETA(DisplayName = "Equip")
};


class UStaticMeshComponent;
class USphereComponent;



UCLASS()
class C251120_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Data)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EItemType ItemType = EItemType::Equip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSubclassOf<class AItemBase> ItemTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsInfinity : 1;
};
