// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Defence.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/BehaviorInterface.h"

UBTTask_Defence::UBTTask_Defence() : m_DurationTime(3.f)
{
	NodeName = TEXT("Defense");
}

EBTNodeResult::Type UBTTask_Defence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	
	// Set Defense State
	BI->OnDefense();
	FOnBehaviorDefenseEnd Delegate;
	Delegate.AddLambda([&]()
		{
			// ::NPC::안에서 제어할 때는 여기서 쓴다.
			if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == true)
			{
				// 만약에 타이머가 끝나기 전에 취소될 경우 타이머를 삭제해준다.
				GetWorld()->GetTimerManager().ClearTimer(m_hDuration);
			}
			else
			{
				// 타이머가 끝났으면 아무일도 일어나지 않는다.
				return;
			}
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			_BI->StopDefense();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetDefenseDelegate(Delegate);

	// Change Delegate : 갑자기 상태가 바뀌었을때 호출 되도록 
	FOnChangeBehavior ChangeDelegate;
	ChangeDelegate.AddLambda([&]()
		{
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			_BI->StopDefense();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetChangeBehaviorDelegate(ChangeDelegate);

	// 5초후에  방어를 종료하도록 [ 시간은 외부에서 받아서 쓰자 ] 
	if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(m_hDuration, [&]()
			{
				// ::AI::안에서 제어할 때는 여기서 쓴다.
				IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
				_BI->StopDefense();
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}, m_DurationTime, false);
	}



	return EBTNodeResult::InProgress;
}


