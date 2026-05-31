// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_AbilityBase.h"
#include "GA_PlayerAbility.generated.h"

class AGASPlayerCharacter;
class AGASPlayerController;
class UPlayerCombatComponent;


/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_PlayerAbility : public UGA_AbilityBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "GAS|Ability")
	AGASPlayerCharacter* GetPlayerCharacterFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "GAS|Ability")
	AGASPlayerController* GetPlayerControllerFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "GAS|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo() ;
	UFUNCTION(BlueprintPure, Category = "GAS|Ability")
	FGameplayEffectSpecHandle MakePlayerDamageEffectspecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount);

protected:
	TWeakObjectPtr<AGASPlayerCharacter> m_CachedPlayerCharacter;
	TWeakObjectPtr<AGASPlayerController> m_CachedPlayerController;

};
