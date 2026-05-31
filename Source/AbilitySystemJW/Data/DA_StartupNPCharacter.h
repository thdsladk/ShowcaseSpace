// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Header/GlobalEnum.h"
#include "DA_StartupNPCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UDA_StartupNPCharacter : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** Ability System */
	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray <TSubclassOf <UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

};
