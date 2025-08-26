// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Header\DataStruct.h"
#include "Interface/PoolableInterface.h"
#include "MyItemData.generated.h"

/*

	데이터 애셋은 필요 없을듯 하다.
	당분간 struct로만 데이터를 들고있으면 될듯하다. 


*/
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
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	//EItemKind m_Kind;

	// 타입 정보는 데이터 구조체 안에 있다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FItemData m_ItemData;

	// 원본 식별 정보(옵션)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemData")
	FName m_ItemDataRowName;




};

