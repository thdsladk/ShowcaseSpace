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
	// NPC[���� ���] , PC[�÷��̾� ĳ����] �� �ൿ ���¶� ����ȭ�ؾ��Ѵ�. 
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
