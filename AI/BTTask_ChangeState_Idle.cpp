// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState_Idle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"

#include "Interface/BehaviorInterface.h"

UBTTask_ChangeState_Idle::UBTTask_ChangeState_Idle()
{
	NodeName = TEXT("ChangeStateIdle");
}

EBTNodeResult::Type UBTTask_ChangeState_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BI)
		return EBTNodeResult::Failed;

	BI->SetState(0);

	return Result;





}
