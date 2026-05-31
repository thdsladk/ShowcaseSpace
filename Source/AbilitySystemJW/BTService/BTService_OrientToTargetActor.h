// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()
	
protected:
	UBTService_OrientToTargetActor();

#pragma region UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
#pragma endregion

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector m_InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Target")
	float m_RotationInterpSpeed;
};

