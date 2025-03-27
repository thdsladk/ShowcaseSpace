// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToSide.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Kismet/KismetMathLibrary.h"

#include "Interface/BehaviorInterface.h"

UBTTask_MoveToSide::UBTTask_MoveToSide()
{
	NodeName = TEXT("MoveToSide");



}

EBTNodeResult::Type UBTTask_MoveToSide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	APawn* Self = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
	{
		IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(Self);
		BI->SetState(0);
		BI->OnIdle();

		return EBTNodeResult::Failed;
	}





	// 앞에 방향을 더해줌으로 인해서 랜덤한 방향으로 흐르도록 제어
	FVector LookVector = ( Target->GetActorLocation() - Self->GetActorLocation() );
	double Distance = LookVector.Size();
	LookVector.Normalize();
	

	//회전된 방향 계산
	FVector LeftDirection = LookVector.RotateAngleAxis(-90.0f, FVector::UpVector);
	FVector RightDirection = LookVector.RotateAngleAxis(90.0f, FVector::UpVector);
	
	// 두 벡터 사이의 각도 계산 (LeftDirection과 LookVector, RightDirection과 LookVector 모두 계산) 
	float AngleLeft = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookVector, LeftDirection)));
	float AngleRight = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookVector, RightDirection)));
	
	//Debug
	//UE_LOG(LogTemp, Log, TEXT("Left Degree : %d "), AngleLeft);
	//UE_LOG(LogTemp, Log, TEXT("Right Degree : %d "), AngleRight);

	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	
	//Self->LookDirection(-FMath::Sign(LookVector.X) * FMath::Sign(LookVector.Y));
	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(Self);
	
	BI->LookDirection(FMath::Sign(LeftDirection.Y));
	
	//TargetRot.Yaw += 60.f;

	


	//Self->SetActorRotation(FMath::RInterpTo(Self->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	//Self->SetActorRotation(TargetRot);



	OwnerComp.GetAIOwner()->MoveToLocation(Self->GetActorLocation() +(LeftDirection * Distance));

	return EBTNodeResult::Succeeded;
}
