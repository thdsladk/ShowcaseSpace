// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"	// (임시) 이건 여기에 써야할지 조금 고민해보자.
#include "MyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

UENUM(BlueprintType)
enum class EAIPerceptionSense : uint8
{
	EPS_None,
	EPS_Sight,
	EPS_Hearing,
	EPS_Damage,
	EPS_MAX
};
/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void RunAI();
	void StopAI();

	bool AddCommandQueue(uint8 Command);
	bool UseCommandQueue(uint8& Command);
	bool IsEmptyCommandQueue();

	// AI Perception Section
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);
	void HandleSensedSight(AActor* Actor);
	void HandleSensedHearing(AActor* Actor,FVector NoiseLocation);
	void HandleSensedDamage(AActor* Actor);
		
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	static const FName StateKey;
	static const FName BattleCommandKey;
	static const FName NPCModeKey;

	static const FName DestPosKey;
	static const FName HasDestPosKey;

	
		


private:
	void RandomMove();
private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> m_AIPerception;

	TObjectPtr<UAISenseConfig_Sight> m_SightConfig;
	TObjectPtr<UAISenseConfig_Hearing> m_HearingConfig;
	TObjectPtr<UAISenseConfig_Damage> m_DamageSenseConfig;

protected:
	TQueue<uint8> m_CommandQueue;


};
