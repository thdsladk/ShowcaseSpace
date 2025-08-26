// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Header\GlobalEnum.h"
#include "MyAbilityData.generated.h"

class UMyAbilityTask;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FAbilityExclusivity
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly) 
    FGameplayTagContainer RequireToStart;
    UPROPERTY(EditDefaultsOnly) 
    FGameplayTagContainer BlockToStart;
    UPROPERTY(EditDefaultsOnly) 
    FGameplayTagContainer ApplyWhileActive; // ¿¹: State.Attacking
    UPROPERTY(EditDefaultsOnly) 
    FGameplayTagContainer CancelOnApply;    // ¿¹: State.Sprinting
};

UCLASS()
class MYTEST_TOPDOWN_API UMyAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly) 
    FGameplayTag m_AbilityTag;
    UPROPERTY(EditDefaultsOnly) 
    FAbilityExclusivity m_Exclusivity;
    UPROPERTY(EditDefaultsOnly) 
    TMap<EGauge, int32> m_Costs;
    UPROPERTY(EditDefaultsOnly) 
    FGameplayTag m_CooldownTag;
    UPROPERTY(EditDefaultsOnly) 
    float m_CooldownSeconds = 1.0f;
    UPROPERTY(EditDefaultsOnly) 
    bool m_bCancelable = true;
    UPROPERTY(EditDefaultsOnly) 
    int32 m_MaxSimultaneousStacks = 1;

    UPROPERTY(EditDefaultsOnly) 
    TObjectPtr<UAnimMontage> m_pMontage;        

    UPROPERTY(EditDefaultsOnly) 
    TArray<TSubclassOf<UMyAbilityTask>> m_TaskPipeline;

};
