// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckState.generated.h"



/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTService_CheckState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckState();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	uint8 m_PreState = 0;
	uint8 m_PreMode = 0;

};
