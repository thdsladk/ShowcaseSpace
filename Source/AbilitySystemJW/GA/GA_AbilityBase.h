// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Header/GlobalEnum.h"
#include "GA_AbilityBase.generated.h"

class UPawnCombatComponent;
class UAbilitySystemComponent;
class UAnimInstanceBase;
class AActor;

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_AbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UAbilitySystemComponent* GetJWAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UAnimInstanceBase* GetAnimInstanceBaseFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Character|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EJWSuccessType& OutSuccessType);


};
