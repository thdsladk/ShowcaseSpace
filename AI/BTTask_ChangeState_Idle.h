// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeState_Idle.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTTask_ChangeState_Idle : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ChangeState_Idle();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
