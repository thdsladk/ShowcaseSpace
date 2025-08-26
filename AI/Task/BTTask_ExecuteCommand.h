// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteCommand.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTTask_ExecuteCommand : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ExecuteCommand();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	

private:
	void OnNull(UBehaviorTreeComponent& OwnerComp);
	void OnAttack(UBehaviorTreeComponent& OwnerComp);
	void OnSmashAttack(UBehaviorTreeComponent& OwnerComp);
	void OnDefense(UBehaviorTreeComponent& OwnerComp);
	void OnCast(UBehaviorTreeComponent& OwnerComp);
	void OnFallBack(UBehaviorTreeComponent& OwnerComp);
	void OnShout(UBehaviorTreeComponent& OwnerComp);
	void OnEscape(UBehaviorTreeComponent& OwnerComp);


private:
	uint8 m_BattleCommand;
	void(UBTTask_ExecuteCommand::*FuncBehavior[10])(UBehaviorTreeComponent& );


private:
	FTimerHandle m_hDefenseDuration;
	double m_DefenseDurationTime;
};
