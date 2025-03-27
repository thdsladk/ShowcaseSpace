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
	// NPC[몬스터 등등] , PC[플레이어 캐릭터] 의 행동 상태랑 동기화해야한다. 
	enum ENPCState : uint8
	{
		Idle,
		Walk,
		Running,
		Chase,
		Attacking,
		Attacking_End,
		Hit,
		Defense,
		Stunned,
		Detect,
		Battle,
		Alert,
		Die,
		End

	};

public:
	UBTService_CheckState();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
