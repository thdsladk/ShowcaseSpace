// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/BehaviorInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());

	BI->Attack();
	FOnBehaviorAttackEnd Delegate;
	Delegate.AddLambda([&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetAttackDelegate(Delegate);

	return EBTNodeResult::InProgress;
}
