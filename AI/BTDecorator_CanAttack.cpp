// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;
	
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;

	// 여기서 상태 구분을 해줄 필요는 없다 여기는 Battle 상태에서만 넘어오는 구간이다.
	if((Target->GetDistanceTo(CurrentPawn) <= 180.f) )
	{
		float fDistance = Target->GetDistanceTo(CurrentPawn);// Debug

		return bResult;
	}
	else
	{
		return false;
	}



}
