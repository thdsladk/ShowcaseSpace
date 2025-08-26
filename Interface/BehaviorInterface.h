// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Header/BehaviorEnum.h"
// (임시) CombatInterface를 만들면 분리하자 .
#include "Header/CombatEnum.h"
#include "BehaviorInterface.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnBehaviorAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnBehaviorDefenseEnd);
DECLARE_MULTICAST_DELEGATE(FOnBehaviorDetectEnd);
DECLARE_MULTICAST_DELEGATE(FOnBehaviorAlertEnd);
DECLARE_MULTICAST_DELEGATE(FOnBehaviorDeathEnd);

DECLARE_MULTICAST_DELEGATE(FOnUpdateBehavior);
DECLARE_MULTICAST_DELEGATE(FOnChangeBehavior);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeMode,uint8);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeCommand,uint8);



DECLARE_MULTICAST_DELEGATE_OneParam(FChangeAggroGauge,uint8);


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBehaviorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API IBehaviorInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const EBehaviorState GetState() = 0;
	virtual void SetState(const EBehaviorState State) = 0;
	virtual const EBehaviorState GetNextState() = 0;
	virtual void SetNextState(const EBehaviorState State) = 0;
	virtual const ECharacterMode GetMode() = 0;
	virtual void SetMode(const ECharacterMode Mode) = 0;


	//virtual const uint8 GetState() = 0;
	virtual void SetState(const uint8 State) =0;
	//virtual const uint8 GetNextState() =0;
	virtual void SetNextState(const uint8 State) =0;
	//virtual const uint8 GetMode() = 0;
	virtual void SetMode(const uint8 Mode) = 0;
	virtual const uint8 GetAggroGauge() = 0;
	virtual void SetAggroGauge(const uint8 AggroGauge) = 0;
	virtual const float GetAggroGauge_Ratio() = 0;


	// Combat Stat 관련 함수인데 이 함수들은 ... 행동 인터페이스에 있는게 맞을까 ? 
	virtual const ECombatType GetCombatType() = 0;
	virtual const float GetMeleeRadius() = 0;
	virtual const float GetRangedRadius() =0;
	virtual const float GetDetectionRadius() =0;
	virtual const float GetVisibleRadius() = 0;

	virtual void OnIdle() = 0;
	virtual void OnBattle() = 0;
	virtual void Attack() = 0;
	virtual void AttackEnd() = 0;
	virtual void Death() = 0;
	virtual void DeathEnd() = 0;
	virtual void OnHit() = 0;
	virtual void OnDefense() = 0;
	virtual void StopDefense() = 0;
	virtual void Defense_Hit() = 0;
	virtual void OnDetect() = 0;
	virtual void DetectEnd() = 0;
	virtual void OnAlert() = 0;

	virtual void OnCommand(uint8 Command) = 0;

	virtual void BehaviorInterrupted(uint8 State) = 0;

	virtual void LookDirection(float Value) = 0;

	virtual FOnChangeMode& GetChangeModeDelegate() = 0;
	virtual FOnChangeCommand& GetChangeCommandDelegate() = 0;

	virtual void SetAttackDelegate(const FOnBehaviorAttackEnd& Delegate) = 0;
	virtual void SetDefenseDelegate(const FOnBehaviorDefenseEnd& Delegate) = 0;
	virtual void SetDetectDelegate(const FOnBehaviorDetectEnd& Delegate) = 0;
	virtual void SetAlertDelegate(const FOnBehaviorAlertEnd& Delegate) = 0;
	virtual void SetDeathDelegate(const FOnBehaviorDeathEnd& Delegate) = 0;

	virtual void SetChangeBehaviorDelegate(const FOnChangeBehavior& Delegate) = 0;
	virtual void SetUpdateBehaviorDelegate(const FOnUpdateBehavior& Delegate) = 0;
	virtual void SetChangeModeDelegate(const FOnChangeMode& Delegate) = 0;

	virtual bool IsOnTriggerEscape() = 0;

};
