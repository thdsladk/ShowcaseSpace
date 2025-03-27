// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToDirection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Kismet/KismetMathLibrary.h"

#include "Interface/BehaviorInterface.h"

UBTTask_MoveToDirection::UBTTask_MoveToDirection()
{
	NodeName = TEXT("MoveToDirection");
}

EBTNodeResult::Type UBTTask_MoveToDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Self = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;


	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
	{
		IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(Self);
		BI->SetState(0);
		BI->OnIdle();

		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Type();
}
