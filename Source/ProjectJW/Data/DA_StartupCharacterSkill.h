// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Header/GlobalEnum.h"
#include "DA_StartupCharacterSkill.generated.h"

class UGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECTJW_API UDA_StartupCharacterSkill : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** Ability System */
	UPROPERTY(EditAnywhere, Category = "SkillAbility")
	TArray <TSubclassOf <UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "SkillAbility")
	TMap<EInputAbilityID, TSubclassOf<UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = "SkillAbility")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;
};
