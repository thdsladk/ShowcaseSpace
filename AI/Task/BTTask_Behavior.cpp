// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Behavior.h"

/*
	�ൿ ������ ������ ����� Task

	Character State

*/

UBTTask_Behavior::UBTTask_Behavior()
{
	NodeName = TEXT("Behavior");
}

EBTNodeResult::Type UBTTask_Behavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return Result;
}
