// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Dead.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/BehaviorInterface.h"

UBTTask_Dead::UBTTask_Dead()
{
	NodeName = TEXT("Dead");
}

EBTNodeResult::Type UBTTask_Dead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (BI == nullptr)
		return EBTNodeResult::Failed;


	// 죽음 상태는 단 한번만 이루어지니까. 
	FOnBehaviorDeathEnd Delegate;
	Delegate.AddLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetDeathDelegate(Delegate);
	//OwnerComp.PauseLogic(TEXT("Paused by Monster"));
	

	return EBTNodeResult::InProgress;
}
