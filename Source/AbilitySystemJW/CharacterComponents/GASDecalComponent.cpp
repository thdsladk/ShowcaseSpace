// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/GASDecalComponent.h"
#include "Actor/AOEBase.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

UGASDecalComponent::UGASDecalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGASDecalComponent::BeginPlay()
{
	Super::BeginPlay();

	// (임시) AOEBase 추가	
	AddAOEBase();

}

void UGASDecalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = 0; i< m_AOEBase.Num();i++)
	{
		if (m_IsVisibleAOEBase.IsEmpty()) { return; }

		if (m_IsVisibleAOEBase[i] == true)
		{
			// !!! 반드시 플레이어에서 사용해야하는 기능이다. !!! 
			APawn* MyPawn = CastChecked<APawn>(GetOwner());
			APlayerController* PC = CastChecked<APlayerController>(MyPawn->GetController());
			FVector Position = UJWFunctionLibrary::ComputePositionFromMouse(PC);
			m_AOEBase[i]->SetActorLocation(Position);
		}
	}
}

void UGASDecalComponent::AddAOEBase()
{
	AAOEBase* AOEActor = GetWorld()->SpawnActor<AAOEBase>(m_AOEClass);
	if (AOEActor)
	{
		//AOEActor->SetDecalMaterial(GetDecalMaterial());
		//AOEActor->SetDecalSize(GetDecalSize());
		AOEActor->SetLifeSpan(0.0f);
		AOEActor->SetActorHiddenInGame(false);
		m_AOEBase.Add(AOEActor);
		m_IsVisibleAOEBase.Add(false);
	}
}

void UGASDecalComponent::SetVisibleAOEBase(int32 Index, bool IsVisible)
{
	if (m_AOEBase.IsValidIndex(Index))
	{
		m_AOEBase[Index]->SetActorHiddenInGame(!IsVisible);
		m_IsVisibleAOEBase[Index] = IsVisible;
	}
}
