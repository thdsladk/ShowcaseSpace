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
			// ::NPC::�ȿ��� ������ ���� ���⼭ ����.
			if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == true)
			{
				// ���࿡ Ÿ�̸Ӱ� ������ ���� ��ҵ� ��� Ÿ�̸Ӹ� �������ش�.
				GetWorld()->GetTimerManager().ClearTimer(m_hDuration);
			}
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			_BI->StopDefense();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	// 5���Ŀ�  �� �����ϵ��� [ �ð��� �ܺο��� �޾Ƽ� ���� ] 
	if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(m_hDuration, [&]()
			{
				// ::AI::�ȿ��� ������ ���� ���⼭ ����.
				IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
				_BI->StopDefense();
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}, m_DurationTime, false);
	}

	BI->SetDefenseDelegate(Delegate);


	return EBTNodeResult::InProgress;
}


