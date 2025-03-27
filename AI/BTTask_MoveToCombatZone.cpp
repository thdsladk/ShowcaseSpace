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
	
	// Monster 클래스단으로 캐스팅 하는 이유는 이동관련 함수를 써야해서. 
	auto Self = Cast<ANPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;


	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;


	/*
	거리가 공격 
	자신과 목표의 거리만큼 뒤로 민다. 

	축은 X , Y 축만 쓰고 Z(높이)는 안써서 날려도 되지만 서로 높이가 다른경우도 감안해서 일단 살려둔다. [ Foward 벡터로 이동 방향을 정하고있으니 ] 
	*/
	FVector SelfPos = Self->GetActorLocation();
	FVector TargetPos = Target->GetActorLocation();

	FVector DirVector = SelfPos - TargetPos;


	SelfPos += DirVector;
	double Dist = FVector::Dist(SelfPos, TargetPos);
	double Sign = FMath::Sign(Dist);
	Dist = FMath::Abs(Dist);
	Dist = FMath::Clamp(Dist, Self->GetMeleeRadius() + 0.1, Self->GetDetectionRadius() - 0.1);		// 0.1은 미세하게 범위안으로 더 넣어주기위한 보정치.


	Self->MoveToFoward((Sign * Dist));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("DestPos")), SelfPos);

	// 위는 무조건 성공 // 아래는 마칠방법을 만들어서 성공시키도록 지속상태.
	return Result;
	//return EBTNodeResult::InProgress;
}
