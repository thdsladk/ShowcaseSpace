// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Defence.h"
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
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			_BI->StopDefense();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	// 5초후에  방어를 종료하도록 [ 시간은 외부에서 받아서 쓰자 ] 
	if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(m_hDuration, [&]()
			{
				// ::AI::안에서 제어할 때는 여기서 쓴다.
				IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
				_BI->StopDefense();
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}, m_DurationTime, false);
	}

	BI->SetDefenseDelegate(Delegate);


	return EBTNodeResult::InProgress;
}


