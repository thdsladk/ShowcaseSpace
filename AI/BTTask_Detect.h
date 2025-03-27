// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Detect.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTTask_Detect : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Detect();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	uint8 m_CharacterState = 0U;
};
