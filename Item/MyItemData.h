// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyItemData.generated.h"

UENUM(BlueprintType)
enum class EItemKind : uint8
{
	Equipment = 0,
	Potion,
	Scroll
};
/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MyItemData", GetFName());
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemKind m_Kind;

};

