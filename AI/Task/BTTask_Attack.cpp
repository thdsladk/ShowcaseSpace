// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Attack.h"
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

	// End Delegate
	FOnBehaviorAttackEnd Delegate;
	Delegate.AddLambda([&]()
		{
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetAttackDelegate(Delegate);

	// Change Delegate : 갑자기 상태가 바뀌었을때 호출 되도록 
	FOnChangeBehavior ChangeDelegate;
	ChangeDelegate.AddLambda([&]()
		{
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetChangeBehaviorDelegate(ChangeDelegate);

	return EBTNodeResult::InProgress;
}
