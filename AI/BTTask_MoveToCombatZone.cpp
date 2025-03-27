// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToCombatZone.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Kismet/KismetMathLibrary.h"

//#include "Interface/BehaviorInterface.h"
#include "Character/NPCharacter.h"

UBTTask_MoveToCombatZone::UBTTask_MoveToCombatZone()
{
	NodeName = TEXT("MoveToCombatZone");
}

EBTNodeResult::Type UBTTask_MoveToCombatZone::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Monster Ŭ���������� ĳ���� �ϴ� ������ �̵����� �Լ��� ����ؼ�. 
	auto Self = Cast<ANPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;


	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;


	/*
	�Ÿ��� ���� 
	�ڽŰ� ��ǥ�� �Ÿ���ŭ �ڷ� �δ�. 

	���� X , Y �ุ ���� Z(����)�� �ȽἭ ������ ������ ���� ���̰� �ٸ���쵵 �����ؼ� �ϴ� ����д�. [ Foward ���ͷ� �̵� ������ ���ϰ������� ] 
	*/
	FVector SelfPos = Self->GetActorLocation();
	FVector TargetPos = Target->GetActorLocation();

	FVector DirVector = SelfPos - TargetPos;


	SelfPos += DirVector;
	double Dist = FVector::Dist(SelfPos, TargetPos);
	double Sign = FMath::Sign(Dist);
	Dist = FMath::Abs(Dist);
	Dist = FMath::Clamp(Dist, Self->GetMeleeRadius() + 0.1, Self->GetDetectionRadius() - 0.1);		// 0.1�� �̼��ϰ� ���������� �� �־��ֱ����� ����ġ.


	Self->MoveToFoward((Sign * Dist));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("DestPos")), SelfPos);

	// ���� ������ ���� // �Ʒ��� ��ĥ����� ���� ������Ű���� ���ӻ���.
	return Result;
	//return EBTNodeResult::InProgress;
}
