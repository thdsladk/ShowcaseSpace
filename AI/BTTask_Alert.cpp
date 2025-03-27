// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Alert.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/BehaviorInterface.h"

UBTTask_Alert::UBTTask_Alert()
{
	NodeName = TEXT("Alert");
}

EBTNodeResult::Type UBTTask_Alert::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Self = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;

	IBehaviorInterface* BI = Cast<IBehaviorInterface>(Self);
	if (nullptr == BI)
		return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;



	BI->OnAlert();
	FOnBehaviorAlertEnd Delegate;

	Delegate.AddLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetAlertDelegate(Delegate);




	// �տ� ������ ���������� ���ؼ� ������ �������� �帣���� ����
	FVector LookVector = (Target->GetActorLocation() - Self->GetActorLocation());
	double Distance = LookVector.Size();
	LookVector.Normalize();


	//ȸ���� ���� ���
	FVector LeftDirection = LookVector.RotateAngleAxis(-90.0f, FVector::UpVector);
	FVector RightDirection = LookVector.RotateAngleAxis(90.0f, FVector::UpVector);

	// �� ���� ������ ���� ��� (LeftDirection�� LookVector, RightDirection�� LookVector ��� ���) 
	float AngleLeft = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookVector, LeftDirection)));
	float AngleRight = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookVector, RightDirection)));

	//Debug
	//UE_LOG(LogTemp, Log, TEXT("Left Degree : %d "), AngleLeft);
	//UE_LOG(LogTemp, Log, TEXT("Right Degree : %d "), AngleRight);

	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	BI->LookDirection(FMath::Sign(LeftDirection.Y));



	OwnerComp.GetAIOwner()->MoveToLocation(Self->GetActorLocation() + (LeftDirection * Distance));


	return EBTNodeResult::InProgress;
}

