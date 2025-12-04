// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class C251120_API NetworkUtil
{
public:
	NetworkUtil();
	~NetworkUtil();

	static void Log(UWorld* World,const FString& InString);
};
