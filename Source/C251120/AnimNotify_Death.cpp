// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Death.h"
#include "CChar.h"

FString UAnimNotify_Death::GetNotifyName_Implementation() const
{
	return TEXT("Death");
}

void UAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	ACChar* Char = Cast<ACChar>(MeshComp->GetOwner());
	if (Char)
	{
		Char->Death();
	}
}
