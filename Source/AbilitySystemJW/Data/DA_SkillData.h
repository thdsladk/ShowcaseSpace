// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_SkillData.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UDA_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Name")
	FName SkillName;

    UPROPERTY(EditAnywhere, Category = "Value")
    float Range;

    UPROPERTY(EditAnywhere, Category = "Value")
    float Width;

    UPROPERTY(EditAnywhere, Category = "Value")
    float AttackRate;

    UPROPERTY(EditAnywhere, Category = "Value")
    float EnergyCost;

    UPROPERTY(EditAnywhere, Category = "Value")
    float CastDelay;

    UPROPERTY(EditAnywhere, Category = "Value")
    float CastTime;

    UPROPERTY(EditAnywhere, Category = "Texture")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, Category = "Particle")
    TSoftObjectPtr<UParticleSystem> Effect;

};
