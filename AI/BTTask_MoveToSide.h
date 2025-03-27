// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToSide.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTTask_MoveToSide : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveToSide();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:


};
