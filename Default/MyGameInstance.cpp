// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#define RandomPoolSize 100

UMyGameInstance::UMyGameInstance()
{

    static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("/Game/TopDown/Data/MyCharacterTable.MyCharacterTable"));
    if (DATA.Succeeded())
    {
        m_MyStats = DATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDATA(TEXT("/Game/TopDown/Data/MonsterTable.MonsterTable"));
    if (MonsterDATA.Succeeded())
    {
        m_MonsterStats = MonsterDATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDATA(TEXT("/Game/TopDown/Data/ItemTable.ItemTable"));
    if (ItemDATA.Succeeded())
    {
        m_ItemTable = ItemDATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> SkillDATA(TEXT("/Game/TopDown/Data/SkillTable.SkillTable"));
    if (SkillDATA.Succeeded())
    {
        m_SkillTable = SkillDATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> CommandDATA(TEXT("/Game/TopDown/Data/CommandTable.CommandTable"));
    if (CommandDATA.Succeeded())
    {
        m_CommandTable = CommandDATA.Object;
    }

}


void UMyGameInstance::Init()
{
    Super::Init();

    if (GetStatData(1) != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->HP);

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Data is Empty "));
    }

    m_RandomPool.Reserve(RandomPoolSize);
    m_RandomPool.Reset(RandomPoolSize);

}

FBaseStatusData* UMyGameInstance::GetStatData(int32 Level)
{
    FString LevelStr = TEXT("Level") + FString::FromInt(Level);
    FBaseStatusData* CharacterData = m_MyStats->FindRow<FBaseStatusData>(*LevelStr, TEXT("Level"));
    //FBaseStatusData* CharacterData = m_MyStats->FindRow<FBaseStatusData>(*FString::FromInt(Level), TEXT("Level"));
    return CharacterData;
}

FMonsterData* UMyGameInstance::GetMonsterStatData(FString Name)
{
    FMonsterData* MonsterData = m_MonsterStats->FindRow<FMonsterData>(FName(Name), TEXT("Monster"));
    return MonsterData;
}

FItemData* UMyGameInstance::GetItemData(int32 ID)
{
    FItemData* ItemData = m_ItemTable->FindRow<FItemData>(*FString::FromInt(ID), TEXT("Item"));
    return ItemData;
}

FSkillData* UMyGameInstance::GetSkillData(FString Name)
{
    FSkillData* SkillData = m_SkillTable->FindRow<FSkillData>(FName(Name), TEXT("Skill"));
    return SkillData;
}

FCommandData* UMyGameInstance::GetCommandData(uint8 Command)
{
    FCommandData* CommandData = m_CommandTable->FindRow<FCommandData>(*FString::FromInt(Command), TEXT("Command"));
    return CommandData;
}

void UMyGameInstance::FillRandomPool()
{
    for (int32 i = 0; i < RandomPoolSize; i++)
    {
        m_RandomPool[i] = FMath::Rand();
    }
}

int32 UMyGameInstance::GetRandomRange(int32 Min, int32 Max)
{
    int32 Result = 0;
    if (m_RandomPool_Index >= m_RandomPool.Num())
    {
        FillRandomPool();
        m_RandomPool_Index = 0;   
    }

    Result = (m_RandomPool[m_RandomPool_Index] / (Max - Min + 1)) + Min;
    m_RandomPool_Index++;
    
    return Result;
}
