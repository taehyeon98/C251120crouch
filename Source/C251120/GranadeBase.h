// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GranadeBase.generated.h"

/**
 * 
 */
UCLASS()
class C251120_API AGranadeBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;
};
