// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ExecuteCommand.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/BehaviorInterface.h"


UBTTask_ExecuteCommand::UBTTask_ExecuteCommand() : m_BattleCommand(0) , m_DefenseDurationTime(5.f)
{
	NodeName = TEXT("ExecuteCommand");

	// ��ɾ� �ൿ ����
	FuncBehavior[0] = &UBTTask_ExecuteCommand::OnNull;
	FuncBehavior[1] = &UBTTask_ExecuteCommand::OnAttack;
	FuncBehavior[2] = &UBTTask_ExecuteCommand::OnSmashAttack;
	FuncBehavior[3] = &UBTTask_ExecuteCommand::OnDefense;
	FuncBehavior[4] = &UBTTask_ExecuteCommand::OnCast;
	FuncBehavior[5] = &UBTTask_ExecuteCommand::OnFallBack;
	FuncBehavior[6] = &UBTTask_ExecuteCommand::OnShout;
	FuncBehavior[7] = &UBTTask_ExecuteCommand::OnEscape;

}

EBTNodeResult::Type UBTTask_ExecuteCommand::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); 


	m_BattleCommand = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("BattleCommand"));

	/*
		�̷��� �����ϱ� ���ؼ� Ư���ൿ�� �����߿� ĵ���� ��츦 ����ؾ��Ѵ� .
		�����߿� ������ ������ Attack���� Hit���¿� ������ �ǰ�   �ɵ��� �ൿ���� ������ �ൿ���� �Ű�����.


		���� :: ������ �ڵ������ �� ������� �������� Behavior Tree �ܿ��� ... �ؼ��� ��������� ������ ���� �� �ִ�.
	*/

	(this->*FuncBehavior[m_BattleCommand])(OwnerComp);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_ExecuteCommand::OnNull(UBehaviorTreeComponent& OwnerComp)
{
}

void UBTTask_ExecuteCommand::OnAttack(UBehaviorTreeComponent& OwnerComp)
{
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (BI == nullptr)
		return;

	BI->Attack();
	FOnBehaviorAttackEnd Delegate;
	Delegate.AddLambda([&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetAttackDelegate(Delegate);
}

void UBTTask_ExecuteCommand::OnSmashAttack(UBehaviorTreeComponent& OwnerComp)
{
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (BI == nullptr)
		return;

	//BI->SmashAttack();
	FOnBehaviorAttackEnd Delegate;
	Delegate.AddLambda([&]()
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetAttackDelegate(Delegate);
}

void UBTTask_ExecuteCommand::OnDefense(UBehaviorTreeComponent& OwnerComp)
{
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (BI == nullptr)
		return;

	// Set Defense State
	BI->OnDefense();
	FOnBehaviorDefenseEnd Delegate;
	Delegate.AddLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	// 5���Ŀ�  �� �����ϵ��� [ �ð��� �ܺο��� �޾Ƽ� ���� ] 
	if (GetWorld()->GetTimerManager().IsTimerActive(m_hDefenseDuration) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(m_hDefenseDuration, [&]() { FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); }, m_DefenseDurationTime, false);
	}

	BI->SetDefenseDelegate(Delegate);

}

void UBTTask_ExecuteCommand::OnCast(UBehaviorTreeComponent& OwnerComp)
{

}

void UBTTask_ExecuteCommand::OnFallBack(UBehaviorTreeComponent& OwnerComp)
{
}

void UBTTask_ExecuteCommand::OnShout(UBehaviorTreeComponent& OwnerComp)
{
}

void UBTTask_ExecuteCommand::OnEscape(UBehaviorTreeComponent& OwnerComp)
{
}
