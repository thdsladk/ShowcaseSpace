// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyGameInstance.h"
#include "MyGameSingleton.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogMyGameSingleton, Error, All);

/**
* << 레거시 코드 >> 
 * 게임 콘텐츠 데이터 관련해서 제어해줄때 사용하자. 
 *  추신 : GameInstance 클래스는 콘텐츠보다는 어플리케이션 데이터를 다루는 쪽으로 사용 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UMyGameSingleton();

	static UMyGameSingleton& Get();

	FORCEINLINE FBaseStatusData GetCharacterStat(int32 InLevel) const { return m_CharacterStatTable.IsValidIndex(InLevel - 1) ? m_CharacterStatTable[InLevel - 1] : FBaseStatusData(); }

	UPROPERTY()
		int32 m_CharacterMaxLevel;



protected:
	TArray<FBaseStatusData> m_CharacterStatTable;


};
