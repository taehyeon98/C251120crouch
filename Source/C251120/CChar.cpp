// Fill out your copyright notice in the Description page of Project Settings.


#include "CChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACChar::ACChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->AddRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->AddRelativeRotation(FRotator(0, -90.0f, 0));

	
}

// Called when the game starts or when spawned
void ACChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ACChar::CanCrouch() const
{
	return true;
}

void ACChar::Move(float Forward, float Right)
{
	const FRotator CharRotation = FRotator(GetController()->GetControlRotation());
	const FRotator CharRotationYaw= FRotator(0, CharRotation.Yaw, 0);
	
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CharRotationYaw);
	AddMovementInput(ForwardVector, Forward);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(CharRotationYaw);
	AddMovementInput(RightVector, Right);
}

void ACChar::Aim(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

