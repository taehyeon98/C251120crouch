// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/DecalComponent.h"
#include "BaseDamageType.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(10, 10, 5));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->MaxSpeed = 8000.0f;
	Movement->InitialSpeed = 8000.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentHit.AddDynamic(this, &AProjectileBase::ProcessComponentHit);
	SetLifeSpan(5.0f);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::ProcessBeginOverlap(AActor* OverlapedActor, AActor* OtherActor)
{
	UDecalComponent* MadeDecal = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		Decal,
		FVector(5, 5, 5),
		HitResult.ImpactPoint,
		HitResult.ImpactNormal.Rotation(),
		5.f
	);
	
	MadeDecal->SetFadeScreenSize(0.005f);
	//HitResult.GetActor();
			//RPG
			/*UGameplayStatics::ApplyDamage(
				HitResult.GetActor(),
				50,
				GetController(),
				this,
				UBaseDamageType::StaticClass()
			);
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());*/

	
	//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());

	////¹üÀ§°ø°Ý, ÆøÅº
	//UGameplayStatics::ApplyRadialDamage(
	//	HitResult.GetActor(), 
	//	50, 
	//	HitResult.ImpactPoint,
	//	300.0f, 
	//	UBaseDamageType::StaticClass(),
	//	IngnoreActors,
	//	this,
	//	GetController(),
	//	true
	//);
	//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());
}

void AProjectileBase::ProcessComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SpawnHitEffect(Hit);

	APawn* Pawn = Cast<APawn>(GetOwner()->GetOwner());


	//ÃÑ½î´Â µ¥¹ÌÁö

	UGameplayStatics::ApplyPointDamage(
		HitResult.GetActor(),
		10,
		-HitResult.ImpactNormal,
		HitResult,
		Pawn->GetController(),
		this,
		UBaseDamageType::StaticClass()
	);
}

void AProjectileBase::SpawnHitEffect(FHitResult Hit)
{
	if (Decal)
	{
		UDecalComponent* MadeDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			Decal,
			FVector(5, 5, 5),
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			5.f
		);

		if (MadeDecal)
		{
			MadeDecal->SetFadeScreenSize(0.005f);
		}
	}

	
}

