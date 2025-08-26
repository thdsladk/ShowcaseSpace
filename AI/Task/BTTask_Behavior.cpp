// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Behavior.h"

/*
	행동 관련한 실행을 담당할 Task

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
