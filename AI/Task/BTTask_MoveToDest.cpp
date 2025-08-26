// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveToDest.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToDest::UBTTask_MoveToDest()
{
	NodeName = TEXT("MoveToDest");
}

EBTNodeResult::Type UBTTask_MoveToDest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* Self = OwnerComp.GetAIOwner()->GetPawn();
	
	FVector CurrentPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::DestPosKey);
	
	bool Equal = CurrentPos.Equals(Self->GetActorLocation());
	if(true == Equal)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::HasDestPosKey,false);
	}

	return Result;

}
