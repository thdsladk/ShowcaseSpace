// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RandomSystem.generated.h"

/**
 *  총량을 넣어서 그에 대한 확률 구하는 기능 ( 기본값 100 )  
	조금 더 고민해서 보편적으로 쓰는 기능으로 확장해 나가자.

	가중치를 받을 때 정수형으로 받는것은 최대한 오차값이 적게 받으려고 하고있다. 
 */
UCLASS()
class MYTEST_TOPDOWN_API URandomSystem : public UObject
{
	GENERATED_BODY()

public:
	URandomSystem();

public:
	int32 PickRandom(int32 TotalSize = 100);

	void AddElement(int32 Key, int32 Weight = 1);
	void RemoveElement(int32 Key);
	
	void AddList(TMap<int32,int32> List);
	void RemoveList(TMap<int32,int32> List);
	
	void Empty() { m_List.Empty(); }
	bool IsEmpty() { return m_List.IsEmpty(); }

protected:
	TMap<int32,int32> m_List;
	double m_TotalWeight = 0.0;

};
