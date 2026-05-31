// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GASAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBlackboardData;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AGASAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGASAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
#pragma region IGenericTeamAgent Interface
public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	void RunAI();
	void StopAI();

private:
	void ComputeViewAngle(const AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* m_EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBlackboardData> m_BBAsset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> m_BTAsset;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	uint8 m_bEnableDetourCrowdAvoidance : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 m_DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float m_CollisionQueryRange = 600.f;

};
