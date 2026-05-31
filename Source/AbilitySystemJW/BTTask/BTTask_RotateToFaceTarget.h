// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

/**
 * 
 */

struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

UCLASS()
class ABILITYSYSTEMJW_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UBTTask_RotateToFaceTarget();

#pragma region UBTNode Interface
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
#pragma endregion

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const;
	float CalculateAngleDiff(APawn* QueryPawn, AActor* TargetActor) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float m_AnglePrecision;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float m_RotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector m_InTargetToFaceKey;
};
