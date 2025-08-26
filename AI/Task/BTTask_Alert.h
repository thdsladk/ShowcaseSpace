// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Alert.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTTask_Alert : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Alert();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	//FVector CalculateDirection();

	int32 m_Direction : 1;
	int32 m_RandomSeed =0;
	FRandomStream m_RandomStream;

	float m_AngleRad = 0.f;
	double m_Radius = 0.0;
	float m_Speed = 1000.f;

	uint8 m_IsPlaying : 1;
};
