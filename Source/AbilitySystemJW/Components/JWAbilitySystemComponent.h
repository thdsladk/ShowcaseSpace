// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "JWAbilitySystemComponent.generated.h"

/**
 * 
 *			< 보류 중인 기능 >
 */
UCLASS()
class ABILITYSYSTEMJW_API UJWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	//void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	//void OnAbilityInputReleased(const FGameplayTag& InInputTag);
	//
	//UFUNCTION(BlueprintCallable, Category = "JW|Ability", meta = (ApplyLevel = "1"))
	//void GrantWeaponAbilities(const TArray<FHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
	//
	//UFUNCTION(BlueprintCallable, Category = "JW|Ability")
	//void RemovedGrantedWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "JW|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
