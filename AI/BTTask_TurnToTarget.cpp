// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Self = (OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Self->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Self->SetActorRotation(FMath::RInterpTo(Self->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 100.0f));

	return EBTNodeResult::Succeeded;
}
