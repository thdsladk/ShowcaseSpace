// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJW_API UComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString m_MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 m_MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float m_FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> m_EffectiveFrameCount;
};
