// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "InstancedStruct.h"
#include "Header\DataStruct.h"

#include "ItemStruct.generated.h"

/*
USTRUCT(BlueprintType)
struct FItemDataBase_Test
{
	GENERATED_BODY()



};

USTRUCT(BlueprintType)
struct FEquipmentData_Test : public FItemData_Test
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentPart EquipmentPart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* EquipmentMesh;

};


USTRUCT(BlueprintType)
struct FItemData_Test : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;									// EItemType로 대체될 예정 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseStatusData Status;
	// TSoftObjectPtr을 통해서 지연 시켜야할 대상 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/Script/MyTest_TopDown.FItemDataBase_Test"))
	FInstancedStruct ItemProperty;
};
*/