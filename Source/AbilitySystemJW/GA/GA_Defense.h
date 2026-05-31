// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Defense.generated.h"

class ATA_Trace;
class ACharacterBase;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
class UAT_Trace;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_Defense : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Defense();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UFUNCTION()
	void OnCompleteCallback();
	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION()
	void OnHitEventCallback(FGameplayEventData Payload);


protected:
	FName GetNextSection();

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

private:
	void ComputeMotionWarping(ACharacter* Character);

protected:
	UPROPERTY(EditAnywhere, Category = "GA|Property")
	uint8 m_IsDefending : 1;


	UPROPERTY(EditAnywhere, Category = "GA|TargetActor")
	TSubclassOf<ATA_Trace> m_TargetActorClass;

	UPROPERTY(EditAnywhere, Category = "GA|GE")
	TArray<TSubclassOf<UGameplayEffect>> m_ParryDamageEffects_Target;

	UPROPERTY(EditAnywhere, Category = "GA|GE")
	TArray<TSubclassOf<UGameplayEffect>> m_ParryDamageEffects_Self;


	TObjectPtr<UAbilityTask_PlayMontageAndWait> m_PlayDefenseTask;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> m_WaitHitEventTask;
	TObjectPtr<UAT_Trace> m_ParryTraceTask;

};
