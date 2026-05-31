// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Header/GlobalEnum.h"
#include "DA_StartupCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;


/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UDA_StartupCharacter : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

public:
	/** Ability System */
	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray <TSubclassOf <UGameplayAbility>> StartAbilities;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<EInputAbilityID, TSubclassOf<UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayAbility> SkillAbilityClass;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

};
