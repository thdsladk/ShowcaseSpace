// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterComponents/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class AGASPlayerWeaponBase;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "GAS|Combat")
	AGASPlayerWeaponBase* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

};
 