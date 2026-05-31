// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_HitCheck.generated.h"

class ATA_Base;
class UAT_WeaponCollision;
/**
 * 블루프린트로 GameplayEffect랑 GameplayCue를 세팅해주도록하자.
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_HitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_HitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

protected:

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void EndHitCheck(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	//UPROPERTY(EditAnywhere, Category = "GAS")
	//TSubclassOf<UGameplayEffect> m_AttackDamageEffect;
	//
	//UPROPERTY(EditAnywhere, Category = "GAS")
	//TSubclassOf<UGameplayEffect> m_AttackBuffEffect;

	// 레벨 스케일링
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Config")
	float m_CurrentLevel;

	FGameplayTagContainer m_ReceivedTags;


	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> m_DamageEffects;

	// 버프 효과들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> m_BuffEffects;

	// 실행할 GameplayCue들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Cues", meta = (Categories = "GameplayCue"))
	TArray<FGameplayTag> m_GameplayCues;
	UPROPERTY(EditAnywhere, Category = "TargetActor")
	TSubclassOf<ATA_Base> m_TargetActorClass;


	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> m_DamageFontClass;

	UPROPERTY(EditAnywhere, Category = "EndTag")
	FGameplayTag m_EndTag;

	TWeakObjectPtr<UAT_WeaponCollision> m_AttackWeaponTask;
	TArray<TWeakObjectPtr<AActor>> m_HitOverlappedActors;
};
