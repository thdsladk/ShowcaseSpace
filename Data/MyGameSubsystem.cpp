// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyGameSubsystem.h"

#include "UI/DamageFontWidget.h"
#include "Interface/PoolableInterface.h"

//#include "Item/MyItemData.h"
/* (의문)과연 ... 게임 인스턴스에서 아이템 헤더를 아는게 맞는가 ? */
#include "Item/MyItem.h"


UMyGameSubsystem::UMyGameSubsystem()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> CharacterInfo(TEXT("/Game/TopDown/Data/CharacterTable.CharacterTable"));
    if (CharacterInfo.Succeeded())
    {
        m_CharacterInfo = CharacterInfo.Object;
    }

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
        for (auto Name : m_ItemTable->GetRowNames())
        {
            FItemData* ItemData = m_ItemTable->FindRow<FItemData>(Name, TEXT("ItemTable"));
            if (ItemData)
            {
                m_ItemTable_Index.Add(ItemData);
			}
        }
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> SkillElementDATA(TEXT("/Game/TopDown/Data/SkillElementTable.SkillElementTable"));
    if (SkillElementDATA.Succeeded())
    {
        m_SkillElementTable = SkillElementDATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> CommandDATA(TEXT("/Game/TopDown/Data/CommandTable.CommandTable"));
    if (CommandDATA.Succeeded())
    {
        m_CommandTable = CommandDATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> SkillDATA(TEXT("/Game/TopDown/Data/SkillTable.SkillTable"));
    if (SkillDATA.Succeeded())
    {
        m_SkillTable = SkillDATA.Object;
    }

    static ConstructorHelpers::FClassFinder<UDamageFontWidget> DFW(TEXT("/Game/TopDown/Blueprints/BP_DamageFont.BP_DamageFont_C"));
    if (DFW.Succeeded())
    {
        m_DamageFont = DFW.Class;
    }


    // 범용 오브젝트 풀
    m_GameObjectPool.Init(nullptr, 100);
    // 발사체 관련 풀
    m_ProjectileActorPool.Init(nullptr, 100);
    // 아이템 관련 풀
    m_ItemActorPool.Init(nullptr, 100);
    // 파티클 관련 풀
    m_ParticleActorPool.Init(nullptr, 100);
    // 스킬 원소 관련 풀
    m_SkillActorPool.Init(nullptr, 100);
    // 데미지 폰트 풀
    m_DamageFontPool.Init(nullptr, 100);
}

void UMyGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    if (GetStatData(1) != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->HP);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Data is Empty "));
    }

    // Create DamageFont Pool
    int32 Num = 0;
    for (auto& Elem : m_DamageFontPool)
    {
        Elem = CreateWidget(GetWorld(), m_DamageFont, *FString::Printf(TEXT("DamageFont_%d"), Num++));
        if (Elem.IsValid() == true)
        {
            CastChecked<IPoolableInterface>(Elem)->ReleaseObject();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Element is Null "));
        }
    }

}

void UMyGameSubsystem::Deinitialize()
{
	Super::Deinitialize();
    // Clear all pools
    for (auto& Elem : m_GameObjectPool)
    {
        if (Elem.IsValid())
        {
            Elem->ConditionalBeginDestroy();
        }
    }
    m_GameObjectPool.Empty();
    for (auto& Elem : m_ProjectileActorPool)
    {
        if (Elem.IsValid())
        {
            Elem->ConditionalBeginDestroy();
        }
    }
    m_ProjectileActorPool.Empty();
    for (auto& Elem : m_ItemActorPool)
    {
        if (Elem.IsValid())
        {
            Elem->ConditionalBeginDestroy();
        }
    }
    m_ItemActorPool.Empty();
    for (auto& Elem : m_ParticleActorPool)
    {
        if (Elem.IsValid())
        {
            Elem->ConditionalBeginDestroy();
        }
    }
    m_ParticleActorPool.Empty();
    for (auto& Elem : m_SkillActorPool)
    {
        if (Elem.IsValid())
        {
            Elem->ConditionalBeginDestroy();
        }
    }
    m_SkillActorPool.Empty();
    for (auto& Elem : m_DamageFontPool)
    {
        if (Elem.IsValid())
        {
            Elem->ConditionalBeginDestroy();
        }
    }
	m_DamageFontPool.Empty();
  
}

const FCharacterInfo* UMyGameSubsystem::GetCharacterInfo(FString Name)
{
    if (m_CharacterInfo.IsValid() == false)
        return nullptr;
    FCharacterInfo* CharacterInfo = m_CharacterInfo->FindRow<FCharacterInfo>(FName(Name), TEXT("CharacterInfo"));
    return CharacterInfo;
}

const FBaseStatusData* UMyGameSubsystem::GetCharacterInfo_StatData(FString Name)
{
    if (m_CharacterInfo.IsValid() == false)
        return nullptr;
    FCharacterInfo* CharacterInfo = m_CharacterInfo->FindRow<FCharacterInfo>(FName(Name), TEXT("CharacterInfo"));
    return &CharacterInfo->Stat;
}

USkeletalMesh* UMyGameSubsystem::GetCharacterInfo_MeshData(FString Name)
{
    if (m_CharacterInfo.IsValid() == false)
		return nullptr;
    FCharacterInfo* CharacterInfo = m_CharacterInfo->FindRow<FCharacterInfo>(FName(Name), TEXT("CharacterInfo"));
    return CharacterInfo->SkeletalMesh;
}

const FEquipmentInfo* UMyGameSubsystem::GetCharacterInfo_EquipmentInfo(FString Name)
{
    if (m_CharacterInfo.IsValid() == false)
		return nullptr;
    FCharacterInfo* CharacterInfo = m_CharacterInfo->FindRow<FCharacterInfo>(FName(Name), TEXT("CharacterInfo"));
    return &CharacterInfo->EquipmentInfo;
}

const FBaseStatusData* UMyGameSubsystem::GetStatData(int32 Level)
{
    if (m_MyStats.IsValid() == false)
		return nullptr;
	FString LevelStr = TEXT("Level") + FString::FromInt(Level);
	FBaseStatusData* CharacterData = m_MyStats->FindRow<FBaseStatusData>(*LevelStr, TEXT("Level"));
	return CharacterData;
}

const FNPCData* UMyGameSubsystem::GetNPCStatData(FString Name)
{
    if (m_MonsterStats.IsValid() == false)
		return nullptr;
	FNPCData* MonsterData = m_MonsterStats->FindRow<FNPCData>(FName(Name), TEXT("Monster"));
	return MonsterData;
}

const FItemData* UMyGameSubsystem::GetItemData(FString Name)
{
    if (m_ItemTable.IsValid() == false)
		return nullptr;
	FItemData* ItemData = m_ItemTable->FindRow<FItemData>(FName(Name), TEXT("Item"));
	return ItemData;
}

const FItemData* UMyGameSubsystem::GetItemData(int32 ID)
{
    if (m_ItemTable.IsValid() == false)
        return nullptr;
    return m_ItemTable_Index[ID];
}

const FSkillElementData* UMyGameSubsystem::GetSkillElementData(FString Name)
{
	FSkillElementData* SkillElementData = m_SkillElementTable->FindRow<FSkillElementData>(FName(Name), TEXT("SkillElement"));
	return SkillElementData;
}

const FSkillData* UMyGameSubsystem::GetSkillData(int32 ID)
{
    FSkillData* SkillData = m_SkillTable->FindRow<FSkillData>(*FString::FromInt(ID), TEXT("Skill"));
    return SkillData;
}

const FCommandData* UMyGameSubsystem::GetCommandData(uint8 Command)
{
	FCommandData* CommandData = m_CommandTable->FindRow<FCommandData>(*FString::FromInt(Command), TEXT("Command"));
	return CommandData;
}

const int32 UMyGameSubsystem::GetItemTableSize()
{
    return m_ItemTable->GetRowNames().Num();
}

UUserWidget* UMyGameSubsystem::GetDamageFont()
{
    // 나중에 완전 탐색을 안하고 더빠르게 찾는 방법을 강구해 보자..
    for (auto Elem : m_DamageFontPool)
    {
        if (Elem != nullptr)
        {
            IPoolableInterface* PoolableInterface = CastChecked<IPoolableInterface>(Elem);
            if (PoolableInterface->IsUseableObject() == true)
            {
                PoolableInterface->AcquireObject();
                return Elem.Get();
            }
            else
            {
                continue;
            }
        }
        else
        {
            // 중간에 nullptr이 나오면 값이 비어 있다는 것이다.
            return nullptr;
        }
    }

    // ObjectPool is Fully in Use.
    UE_LOG(LogTemp, Warning, TEXT("All objects are currently in use."));
    return nullptr;
}

AActor* UMyGameSubsystem::GetItemActor(FItemRuntimeData ItemRuntimeData, FTransform Transform)
{
    if (GetWorld() == nullptr)
        return nullptr;
    
    for (auto Elem : m_ItemActorPool)
    {
        if (Elem != nullptr)
        {
            AMyItem* Item = CastChecked<AMyItem>(Elem);
            if (Item->IsUseableObject() == true)
            {
                Item->InitData(ItemRuntimeData);
                Item->AcquireObject();
                return Elem.Get();
            }
            else
            {
                continue;
            }
            //IPoolableInterface* PoolableInterface = CastChecked<IPoolableInterface>(Elem);
            //if (PoolableInterface->IsUseableObject() == true)
            //{
            //    PoolableInterface->AcquireObject();
            //    return Elem.Get();
            //}
            //else
            //{
            //    continue;
            //}
        }
        else
        {
            // 객체가 아직 없을때 
            Elem = GetWorld()->SpawnActor(AMyItem::StaticClass(), &Transform);
            return Elem.Get();
        }
    }

    // 객체가 모두 사용중일때...
    return nullptr;
}
