// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Header/GlobalEnum.h"
#include "GA_ReactBase.generated.h"

class ATA_Trace;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayTagRemoved;


/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_ReactBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ReactBase();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UFUNCTION()
	void OnCompleteCallback();
	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION()
	void OnRemovedTagCallback();

private:
	void ComputeMotionWarping(ACharacter* Character, const AActor& Instigator);

#pragma region Montage Propertys
protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<ECombatMode,TObjectPtr<UAnimMontage>> m_DefenseReactMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TMap<FGameplayTag,FName> m_SectionNameMap;							// 아직은 단일 세션만을 재생하는 형태로 구현.

#pragma endregion

protected:
	// 레벨 스케일링
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Config")
	float m_CurrentLevel;

	// 버프 효과들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> m_BuffEffects;

	// 실행할 GameplayCue들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Cues")
	TArray<FGameplayTag> m_GameplayCues_Effect;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Config")
	float m_Force = 100.f;


protected:
	TObjectPtr<UAbilityTask_PlayMontageAndWait> m_ReactTask;
	TObjectPtr< UAbilityTask_WaitGameplayTagRemoved> m_TagRemovedTask;

};
