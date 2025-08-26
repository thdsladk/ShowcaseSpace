// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Kismet/KismetMathLibrary.h"

#include "Interface/BehaviorInterface.h"

UBTTask_Detect::UBTTask_Detect()
{
	NodeName = TEXT("Detect");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Detect::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BI)
		return EBTNodeResult::Failed;

	BI->OnDetect();

	FOnBehaviorDetectEnd Delegate;
	Delegate.AddLambda([this, &OwnerComp]()
	{
			auto _BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			if (nullptr == _BI)
				return;
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			this->FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});
	BI->SetDetectDelegate(Delegate);

	return EBTNodeResult::InProgress;
}

void UBTTask_Detect::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto Self = (OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return;
	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return;

	FVector LookVector = Target->GetActorLocation() - Self->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Self->SetActorRotation(FMath::RInterpTo(Self->GetActorRotation(), TargetRot, DeltaSeconds, 200.0f));

}

