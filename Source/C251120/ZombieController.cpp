// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieController.h"
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include "CChar.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPPZombie.h"


AZombieController::AZombieController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->SightRadius = 800.0f;
	Sight->LoseSightRadius = 900.0f;
	Sight->PeripheralVisionAngleDegrees = 45.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void AZombieController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//ensure(RunBTAsset); //Log
	//check(RunBTAsset); //우선 종료 후 로그 확인
	if (RunBTAsset)
	{
		RunBehaviorTree(RunBTAsset);
	}

	//Perception->OnPerceptionUpdated.AddDynamic(this, &AZombieController::ProcessPerception);
	Perception->OnTargetPerceptionForgotten.AddDynamic(this, &AZombieController::ProcessPerceptionForget);
	//Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AZombieController::ProcessActorPerceptionInfo);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieController::ProcessActorPerception);
	SetGenericTeamId(3);
}

void AZombieController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AZombieController::ProcessPerception(const TArray<AActor*>& UpdatedActors)
{
	for (auto Actor : UpdatedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessPerception %s"), *Actor->GetName());
	}
}

void AZombieController::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("ProcessActorPerception %s %s"), *Actor->GetName(),*Stimulus.Type.Name.ToString());
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())//봤을때
		{
			ACChar* Player = Cast<ACChar>(Actor);
			ACPPZombie* Zombie = Cast<ACPPZombie>(GetPawn());
			if (Player && Zombie)
			{
				if (Zombie->GetCurrentState() == EZombieState::Death)
				{
					return;
				}
				//BrainComponent->AIComponent최상위.
				Blackboard->SetValueAsObject(TEXT("Target"), Player);
				SetState(EZombieState::Chase);
				Zombie->SetState(EZombieState::Chase);
				Zombie->ChangeSpeed(400.0f);
			}
		}
		else //못봤을때
		{
			ACChar* Player = Cast<ACChar>(Actor);
			ACPPZombie* Zombie = Cast<ACPPZombie>(GetPawn());
			if (Player && Zombie)
			{
				if (Zombie->GetCurrentState() == EZombieState::Death)
				{
					return;
				}
				Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
				/*Blackboard->SetValueAsVector(TEXT("Destination"), Stimulus.StimulusLocation);*/
				SetState(EZombieState::Normal);
				Zombie->SetState(EZombieState::Normal);
				Zombie->ChangeSpeed(80.0f);
			}
		}
	}
}

void AZombieController::ProcessPerceptionForget(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPerceptionForget %s"), *Actor->GetName());
	//아까 본데 까먹으면
	ACChar* Player = Cast<ACChar>(Actor);
	ACPPZombie* Zombie = Cast<ACPPZombie>(GetPawn());
	if (Player&& Zombie)
	{
		Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
		SetState(EZombieState::Normal);
		Zombie->SetState(EZombieState::Normal);
		Zombie->ChangeSpeed(80.0f);
	}
}

void AZombieController::ProcessActorPerceptionInfo(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPerceptionForget %s"), *(UpdateInfo.Target->GetName()));
}

void AZombieController::SetState(EZombieState NewState)
{
	Blackboard->SetValueAsEnum(TEXT("CurrentState"), (uint8)(NewState));
}
