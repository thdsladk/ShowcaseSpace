// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Header/GlobalEnum.h"
#include "Header/DataStruct.h"
#include "MyGameInstance.generated.h"

/**
 * �� �ܰ��� �����͵��� �����ϴ� �������� �뵵
 * 
 */



UCLASS()
class MYTEST_TOPDOWN_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UMyGameInstance();
	virtual void Init() override;


	// RandomPool
	void FillRandomPool();
	int32 GetRandomRange(int32 Min, int32 Max);

private:


	UPROPERTY()
	TArray<int32> m_RandomPool;
	UPROPERTY()
	int32 m_RandomPool_Index = 0;
	

};
