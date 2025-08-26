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
			// ::NPC::�ȿ��� ������ ���� ���⼭ ����.
			if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == true)
			{
				// ���࿡ Ÿ�̸Ӱ� ������ ���� ��ҵ� ��� Ÿ�̸Ӹ� �������ش�.
				GetWorld()->GetTimerManager().ClearTimer(m_hDuration);
			}
			else
			{
				// Ÿ�̸Ӱ� �������� �ƹ��ϵ� �Ͼ�� �ʴ´�.
				return;
			}
			IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			_BI->StopDefense();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetDefenseDelegate(Delegate);

	// Change Delegate : ���ڱ� ���°� �ٲ������ ȣ�� �ǵ��� 
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

	// 5���Ŀ�  �� �����ϵ��� [ �ð��� �ܺο��� �޾Ƽ� ���� ] 
	if (GetWorld()->GetTimerManager().IsTimerActive(m_hDuration) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(m_hDuration, [&]()
			{
				// ::AI::�ȿ��� ������ ���� ���⼭ ����.
				IBehaviorInterface* _BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
				_BI->StopDefense();
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}, m_DurationTime, false);
	}



	return EBTNodeResult::InProgress;
}


