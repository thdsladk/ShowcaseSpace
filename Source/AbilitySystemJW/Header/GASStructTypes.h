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
struct FSkillDescriptionData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category ="Name")
    FName SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")

    TSoftObjectPtr<UTexture2D> Icon;
};


USTRUCT(BlueprintType)
struct FTargetData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FTargetData() :
		TargetStartPointType(ETargetStartPointType::Character),
		TargetType(ETargetType::Hostile),
		TargetCount(1)
	{
	}


	UPROPERTY(EditAnywhere, Category = "TargetData")
	ETargetStartPointType TargetStartPointType;
	
	UPROPERTY(EditAnywhere, Category = "TargetData")
	ETargetType TargetType;
	
	UPROPERTY(EditAnywhere, Category = "TargetData")
	uint32 TargetCount;

	UPROPERTY(EditAnywhere, Category = "TargetData")
	FVector OffsetFromActor;

	UPROPERTY(EditAnywhere, Category = "TargetData")
	FJWCollisionShape CollisionShape;
};


