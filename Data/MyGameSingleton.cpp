// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyGameSingleton.h"


DEFINE_LOG_CATEGORY(LogMyGameSingleton);

UMyGameSingleton::UMyGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/TopDown/Data/MyCharacterTable.MyCharacterTable")); 
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, m_CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FBaseStatusData*>(Value);
			}
		);
	}

	m_CharacterMaxLevel = m_CharacterStatTable.Num();
	ensure(m_CharacterMaxLevel > 0);
}

UMyGameSingleton& UMyGameSingleton::Get()
{
	// TODO: 여기에 return 문을 삽입합니다.
	UMyGameSingleton* Singleton = CastChecked<UMyGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogMyGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UMyGameSingleton>();
}
