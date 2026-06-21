// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_SkillData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ABILITYSYSTEMJW_API UDA_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	FName SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
    float Width;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
    float AttackRate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
    float EnergyCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
    float CastDelay;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
    float CastTime;

    UPROPERTY(EditAnywhere, Category = "Texture")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, Category = "Particle")
    TSoftObjectPtr<UParticleSystem> Effect;

};
