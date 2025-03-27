// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Kismet/KismetMathLibrary.h"

#include "Interface/BehaviorInterface.h"

UBTTask_Detect::UBTTask_Detect()
{
	NodeName = TEXT("Detect");
}

EBTNodeResult::Type UBTTask_Detect::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BI)
		return EBTNodeResult::Failed;

	BI->OnDetect();

	FOnBehaviorDetectEnd Delegate;
	Delegate.AddLambda([&]()
	{
			auto _BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			if (nullptr == _BI)
				return;
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("State"), _BI->GetState());
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});
	BI->SetDetectDelegate(Delegate);

	return EBTNodeResult::InProgress;
}

