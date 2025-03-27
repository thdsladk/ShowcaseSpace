// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RandomSystem.generated.h"

/**
 *  �ѷ��� �־ �׿� ���� Ȯ�� ���ϴ� ��� ( �⺻�� 100 )  
	���� �� ����ؼ� ���������� ���� ������� Ȯ���� ������.

	����ġ�� ���� �� ���������� �޴°��� �ִ��� �������� ���� �������� �ϰ��ִ�. 
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
