// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Header/GlobalEnum.h"
#include "Header/DataStruct.h"
#include "MyGameInstance.generated.h"

constexpr int32 MaxStatus = 99999;
/**
 * 
 */

// 비트단위로 Flag를 해서 쓸때 사용할려고 ...
USTRUCT()
struct FBitPool
{
	GENERATED_BODY()
public:
	uint8 Element : 1;
};

UCLASS()
class MYTEST_TOPDOWN_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UMyGameInstance();
	virtual void Init() override;

	// Data Function
	FBaseStatusData* GetStatData(int32 Level);
	FMonsterData* GetMonsterStatData(FString Name);
	FItemData* GetItemData(int32 ID);
	FSkillData* GetSkillData(FString Name);
	FCommandData* GetCommandData(uint8 Command);

	// RandomPool
	void FillRandomPool();
	int32 GetRandomRange(int32 Min, int32 Max);

private:

	UPROPERTY()
	class UDataTable* m_MyStats;
	UPROPERTY()
	class UDataTable* m_MonsterStats;
	UPROPERTY()
	class UDataTable* m_ItemTable;
	UPROPERTY()
	class UDataTable* m_SkillTable;
	UPROPERTY()
	class UDataTable* m_CommandTable;


	UPROPERTY()
	TArray<int32> m_RandomPool;
	UPROPERTY()
	int32 m_RandomPool_Index = 0;
	

};
