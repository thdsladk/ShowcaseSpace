// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_AttackHitCheck.generated.h"

class UGameplayEffect;
class ATA_Base;
class UUserWidget;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_AttackHitCheck();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	void ApplyAbilityTask();

protected:
	// 레벨 스케일링
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Config")
	float m_CurrentLevel;
	
	FGameplayTagContainer m_ReceivedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TMap<FGameplayTag,TSubclassOf<UGameplayEffect>> m_DamageEffects;

	// 버프 효과들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> m_BuffEffects;

	// 실행할 GameplayCue들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Cues")
	TArray<FGameplayTag> m_GameplayCues;

	UPROPERTY(EditAnywhere, Category = "Ability|TargetActor")
	TSubclassOf<ATA_Base> m_TargetActorClass;


	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> m_DamageFontClass;



};
