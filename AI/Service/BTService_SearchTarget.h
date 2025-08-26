// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTService_SearchTarget : public UBTService
{
	GENERATED_BODY()


public:
	UBTService_SearchTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

#pragma region DEBUG
protected:
	uint8 m_IsDebug :1;
	uint8 m_CountDebug = 0;
#pragma endregion
};
