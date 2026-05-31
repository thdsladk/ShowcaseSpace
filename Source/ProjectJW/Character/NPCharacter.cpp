// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPCharacter.h"
#include "Engine/AssetManager.h"

ANPCharacter::ANPCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void ANPCharacter::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void ANPCharacter::NPCMeshLoadCompleted()
{

}
