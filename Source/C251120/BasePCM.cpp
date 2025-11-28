// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePCM.h"
#include "CChar.h"

ABasePCM::ABasePCM()
{
}

void ABasePCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	ACChar* Pawn = Cast<ACChar>(GetOwningPlayerController()->GetPawn());
	if (Pawn)
	{
		float TargetZoom = 0;
		if (Pawn->bIsIronSight)
		{
			TargetZoom = 50.0f;
		}
		else
		{
			TargetZoom = 90.0f;
		}
		CurrentZoom = FMath::FInterpTo(CurrentZoom, TargetZoom, DeltaTime, 8.0f);
		SetFOV(CurrentZoom);
	}
}
