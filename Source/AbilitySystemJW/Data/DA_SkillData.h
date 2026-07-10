// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Header/GASStructTypes.h"
#include "DA_SkillData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ABILITYSYSTEMJW_API UDA_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillDescriptionData SkillData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTargetData TargetData;

};
