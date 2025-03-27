// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Escape.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTTask_Escape::UBTTask_Escape()
{
	NodeName = TEXT("Escape");
}

EBTNodeResult::Type UBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Self = OwnerComp.GetAIOwner()->GetPawn();
	if (Self == nullptr)
		return EBTNodeResult::Failed;

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	if ((Target->GetDistanceTo(Self) <= 1200.f) )
	{
		FVector Destination = Self->GetActorLocation() - Target->GetActorLocation();
		Destination.Z = 0.0f;
		
		Destination.Normalize();
		// 반대로 회전
		FRotator TargetRot = FRotationMatrix::MakeFromX(Destination).Rotator();
		Self->SetActorRotation(FMath::RInterpTo(Self->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));
		
		OwnerComp.GetAIOwner()->MoveToLocation(Self->GetActorLocation() +(Destination*100.f));
		float fDistance = Target->GetDistanceTo(Self);// Debug

		return Result;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
