// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_EstimationStatus.h"
#include "MyAIController.h"
#include "Interface/BehaviorInterface.h"
#include "MonsterStatComponent.h"

UBTDeco_EstimationStatus::UBTDeco_EstimationStatus()
{
	NodeName = TEXT("EstimationStatus");
}

bool UBTDeco_EstimationStatus::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Self = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (Self == nullptr)
		return false;

	return (bResult = Self->IsOnTriggerEscape());
}
