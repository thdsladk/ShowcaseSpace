// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Attack.generated.h"

class UComboActionData;
class ACharacterBase;
class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Attack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();
	void StartComboTimer();
	void CheckComboInput();

private:
	void ComputeMotionWarping(ACharacter* Character);

#pragma region Melee Section
protected:
	UPROPERTY()
	TObjectPtr<UComboActionData> m_CurrentComboData;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> m_PlayAttackTask;
protected:
	uint8 m_CurrentCombo = 0;
	FTimerHandle m_ComboTimerHandle;
	uint8 m_HasNextComboInput : 1;
#pragma endregion


protected:
	uint8 m_OriginMovementMode;
};
