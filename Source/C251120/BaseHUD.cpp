// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "Gameframework/CharacterMovementComponent.h"

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();

	int32 Unit = Canvas->SizeX / 100;
	//int32 UnitY = Canvas->SizeY / 100;
	int32 CenterX = Canvas->SizeX / 2;
	int32 CenterY = Canvas->SizeY / 2;
	int32 DrawSize = 1.7f;

	ACharacter* Pawn = Cast<ACharacter>(GetOwningPawn());
	float CurrentSpeed = 0.f;
	float MaxSpeed = 0.f;
	float GapRatio = 0.0f;
	int32 Gap = Unit;
	//int32 GapY = UnitY;


	if (Pawn)
	{
		MaxSpeed = Pawn->GetCharacterMovement()->GetMaxSpeed();
		CurrentSpeed = Pawn->GetCharacterMovement()->Velocity.Size2D();
		GapRatio = CurrentSpeed / MaxSpeed;
	}
	Gap = (int32)((float)Gap * GapRatio);

	Draw2DLine(CenterX - (Unit * DrawSize) - Gap,
		CenterY,
		CenterX - Gap,
		CenterY,
		FColor::Red);

	Draw2DLine(CenterX + Gap,
		CenterY,
		CenterX + (Unit * DrawSize) + Gap,
		CenterY,
		FColor::Red);

	Draw2DLine(CenterX,
		CenterY - (Unit * DrawSize) - Gap,
		CenterX,
		CenterY - Gap,
		FColor::Red);

	Draw2DLine(CenterX,
		CenterY + Gap,
		CenterX,
		CenterY + (Unit * DrawSize) + Gap,
		FColor::Red);
	
}
