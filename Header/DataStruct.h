// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "DataStruct.generated.h"


USTRUCT()
struct FBaseStatusData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WillPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRadius;


	//int32 Zeal;	// 열성 수치 : 전투를 하거나 무언가를 할때마다 올라간다.

	FBaseStatusData operator+(const FBaseStatusData& Other) const
	{
		const int32* const ThisPtr = reinterpret_cast<const	 int32* const>(this);
		const int32* const OtherPtr = reinterpret_cast<const int32* const>(&Other);

		FBaseStatusData Result;
		int32* ResultPtr = reinterpret_cast<int32*>(&Result);
		int32 StatNum = sizeof(FBaseStatusData) / sizeof(int32);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};

USTRUCT()
struct FStatusGaugeData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSP;
};

USTRUCT()
struct FMonsterData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level;

	//Gauge Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SP;

	//Status Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WillPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* LEquipMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* REquipMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RItemID;

};

USTRUCT()
struct FItemData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh;

};

USTRUCT()
struct FSkillData : public  FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EffectCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EffectDurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EffectIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Icon;
};

USTRUCT()
struct FCommandData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERadiusType RadiusType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};

