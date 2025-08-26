// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Header/GlobalEnum.h"
#include "BTService_Decision.generated.h"

/**
 * 
 */


UCLASS()
class MYTEST_TOPDOWN_API UBTService_Decision : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Decision();
	virtual	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

private:
	bool CalculateDestination(UBehaviorTreeComponent& OwnerComp, ERadiusType DestRadiusType);

};
