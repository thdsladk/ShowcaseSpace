// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Header/GASStructTypes.h"
#include "GA_SkillHitCheck.generated.h"

enum class ESkillTargetActorType : uint8
{
	Trace     UMETA(DisplayName = "Trace"),
	MultiTrace UMETA(DisplayName = "MultiTrace"),
	Radius     UMETA(DisplayName = "Radius"),
	End        UMETA(DisplayName = "End")
};
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_SkillHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_SkillHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void OnSkillResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);	
	UFUNCTION()
	void OnCancelledSkillCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	void ApplyAbilityTask();


protected:
	// 레벨 스케일링
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Config")
	float m_CurrentLevel;

	FGameplayTagContainer m_ReceivedTags;

	TEnumAsByte<EGameplayTargetingConfirmation::Type>  m_ConfirmationType;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> m_DamageEffects;

	// 버프 효과들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> m_BuffEffects;

	// 실행할 GameplayCue들
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Cues")
	TMap<FGameplayTag, FGameplayTag> m_GameplayCues;




	ESkillTargetActorType m_TargetActorType;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> m_DamageFontClass;


};
