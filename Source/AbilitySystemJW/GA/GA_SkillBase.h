// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/ActionAbility/GA_ActionAbilityBase.h"
#include "Header/GlobalEnum.h"
#include "Header/GASStructTypes.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GA_SkillBase.generated.h"

class AActor;
class UGameplayEffect;
class AIndicatorBase;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitDelay;
class UAbilityTask_WaitInputPress;
class UAbilityTask_WaitGameplayEvent;
class UDA_SkillData;
class ATA_Base; 
class ATA_Skill;
class ATA_Trace;
class UTargetSystemComponent;
class UMotionWarpingComponent;

/**
 *  만약 여기서 Player관련 기능을 넣게 된다면 CombatMode가 들어가면 PlayerSkillBase로 상속을 내려야할거같다. 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_SkillBase : public UGA_ActionAbilityBase	// UGameplayAbility 
{
	GENERATED_BODY()
public:
	UGA_SkillBase();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	// 구역을 나누자... 1. 스킬 지정 섹션 2. 인디케이터 3. 스킬 실행

protected:
	UFUNCTION()
	void OnCompleteCallback();
	UFUNCTION()
	void OnInterruptedCallback();

	//UFUNCTION()
	//void OnWorldRecticleFinished();
	//UFUNCTION()
	//void OnPressedBtn(FGameplayEventData Payload = FGameplayEventData());

	UFUNCTION()
	void PlaySkill();

	UFUNCTION()
	void PlaySkill_TargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
	UFUNCTION()
	void CancelSkill_TargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

#pragma region Blueprint Functions
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MotionWarping")
	void CalcMotionWarping(ACharacter* Character);
	virtual void CalcMotionWarping_Implementation(ACharacter* Character);

#pragma endregion

#pragma region Montage Propertys

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> m_SkillMontage;

#pragma endregion


protected:
	FTimerHandle m_SkillTimerHandle;
	uint8 m_HasNextInput : 1;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicator", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	TSubclassOf<AIndicatorBase> m_IndicatorClass;
	TObjectPtr<AIndicatorBase> m_Indicator;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Ability|Data")
	TObjectPtr<UDA_SkillData> m_SkillData;

	UPROPERTY(EditAnywhere, Category = "Ability|Data")
	FSkillTargetData SkillTargetData;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effect")
	TSubclassOf<UGameplayEffect> m_InitSkillEffect;

#pragma region TargetActor Members
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetActor")
	TSubclassOf<ATA_Skill> m_TargetActorClass;
	TObjectPtr<ATA_Skill> m_TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetActor")
	TSubclassOf<ATA_Skill> m_SkillTargetActorClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetActor")
	TEnumAsByte<EGameplayTargetingConfirmation::Type>  m_ConfirmationType;

#pragma endregion


protected:
	uint8 m_OriginMovementMode;
	uint8 m_CurrentCombatMode;

#pragma region Ability Flags
protected:
	uint8 m_bPlaySkill : 1;
	UPROPERTY(EditAnywhere, Category = "Ability|Flags")
	bool m_HasMovement = false;

#pragma endregion

protected: 
	UPROPERTY(EditAnywhere, Category = "Ability|Property")
	float m_SkillSpeedRatio;


#pragma region Task Members
protected:
	TObjectPtr<UAbilityTask_PlayMontageAndWait> m_PlaySkillTask;
	TObjectPtr<UAbilityTask_WaitDelay> m_IndicatorTask;
#pragma endregion

protected:
	TWeakObjectPtr<UTargetSystemComponent> m_TargetSystemComp;
};
