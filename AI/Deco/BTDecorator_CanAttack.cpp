// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/BehaviorInterface.h"

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
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(CurrentPawn);
	if (BI == nullptr)
		return false;
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (Target == nullptr)
		return false;

	float fDistance = Target->GetDistanceTo(CurrentPawn);

	switch (BI->GetCombatType())
	{
		case ECombatType::CT_Melee :
		{
			if (fDistance <= BI->GetMeleeRadius())
			{
				return bResult;
			}
			break;
		}
		case ECombatType::CT_Ranged:
		{
			// 원거리 공격은 근거리 공격 사거리보다 가까우면 공격이 안되도록 한다.
			if (fDistance <= BI->GetRangedRadius() || fDistance > BI->GetMeleeRadius())
			{
				return bResult;
			}
			break;
		}

		default :
		{
			break;
		}
	}

	return false;
}
