// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_RangeAttack.generated.h"

class UAbilityTask_PlayMontageAndWait;
class AProjectileBase;


UENUM()
enum class EShootState : uint8
{
	Stop	UMETA(DisplayName = "Draw"),
	Draw	UMETA(DisplayName = "Draw"),
	Charge	UMETA(DisplayName = "Draw"),
	Shot	UMETA(DisplayName = "Draw"),
	End		UMETA(DisplayName = "End"),
};
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_RangeAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_RangeAttack();

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

	FORCEINLINE void IncreaseState(){ m_ShootState = (m_ShootState + 1U) % uint8(EShootState::End); }
	FName GetNextSection();


protected:
	void EndSection();
	void ShootProjectile();

#pragma region Range Section

protected:
	uint8 m_ShootState;
	uint8 m_IsInputRelease : 1;

#pragma endregion

protected:
	UPROPERTY(EditAnywhere,Category = "Projectile")
	TSubclassOf<AProjectileBase> m_ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<UGameplayEffect> m_GaugeEffectClass;

	FActiveGameplayEffectHandle m_GaugeActiveEffectHandle;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> m_PlayRangeAttackTask;

};
