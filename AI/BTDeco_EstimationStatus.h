// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_EstimationStatus.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTDeco_EstimationStatus : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDeco_EstimationStatus();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
