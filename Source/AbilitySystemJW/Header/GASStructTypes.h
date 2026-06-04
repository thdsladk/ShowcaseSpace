// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Header/CollisionStructTypes.h"
#include "GASStructTypes.generated.h"

class UGASPlayerLinked_AnimLayer;

USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGASPlayerLinked_AnimLayer> WeaponAnimLayerToLink;
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefaultRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefaultAttackRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefaultEnergyCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CastDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CastTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)

    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UParticleSystem> Effect;
};


USTRUCT(BlueprintType)
struct FSkillTargetData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "SkillTargetData")
	ETargetStartPointType TargetStartPointType;
	UPROPERTY(EditAnywhere, Category = "SkillTargetData")
	ETargetType TargetType;
	UPROPERTY(EditAnywhere, Category = "SkillTargetData")
	uint32 TargetCount;
	UPROPERTY(EditAnywhere, Category = "SkillTargetData")
	FJWCollisionShape CollisionShape;
};