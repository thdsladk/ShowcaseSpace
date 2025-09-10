// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/AbilityComponent.h"

#include "Ability/MyAbilityInstance.h"
#include "Ability/MyAbilityData.h"

#include "Character/CharacterBase.h"
#include "MyTest_TopDownPlayerController.h"
#include "CharacterComponent\MyStatComponent.h"
#include "AbilityComponent.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

    // (임시) OwnerTag를 임시로 세팅
	m_OwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Mode.Battle")));
	
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (auto& Pair : m_Actives)
    {
        if (Pair.Value)
        {
            Pair.Value->Tick(DeltaTime);
        }
    }
}

void UAbilityComponent::GrantAbility(UMyAbilityData* Data)
{
    if (Data != nullptr && 
        m_Granted.Contains(Data->m_AbilityTag) == false)
    {
        m_Granted.Add(Data->m_AbilityTag, Data);
    }
}

void UAbilityComponent::RevokeByTag(FGameplayTag AbilityTag)
{
    m_Granted.Remove(AbilityTag);
}

EAbilityActivateResult UAbilityComponent::TryActivateByTag(FGameplayTag AbilityTag, FAbilityHandle& OutHandle)
{
    // 태그에서 필요한 부분만 떼어낸 태그로 검색  // (임시) 처음 실패하면 3번째 태그까지도 비교한다. 
    auto FoundData = m_Granted.Find(CutStringToTag(AbilityTag));
    if (FoundData == nullptr)
    {
        FoundData = m_Granted.Find(AbilityTag);
        if (FoundData == nullptr)
        {
            return EAbilityActivateResult::NotGranted;
        }
    }

    UMyAbilityData* AbilityData = *FoundData;
    if (AbilityData != nullptr)
    {
        // 세부 태그를 다시 넣어주는 부분.
        AbilityData->m_AbilityTag = AbilityTag;
    }

	// 태그 조건 검사 ( 필요 태그, 차단 태그 )
    if (m_OwnedTags.HasAll(AbilityData->m_Exclusivity.RequireToStart) == false)
        return EAbilityActivateResult::MissingTags;
    if (m_OwnedTags.HasAny(AbilityData->m_Exclusivity.BlockToStart) == true)
        return EAbilityActivateResult::BlockedTags;

    // 쿨다운 검사
    if (IsOnCooldown(AbilityData->m_CooldownTag))
        return EAbilityActivateResult::OnCooldown;

    // 코스트 검사 
    ACharacterBase* Owner = CastChecked<ACharacterBase>(GetOwner());
    if (Owner->GetStatComponent().CanPay(AbilityData->m_Costs) == false) return EAbilityActivateResult::InsufficientCost;

    // 커밋 (쿨다운 시작)
    m_CooldownsUTC.Add(AbilityData->m_CooldownTag, FPlatformTime::Seconds() + AbilityData->m_CooldownSeconds);

    // 인스턴스 생성
    OutHandle = FAbilityHandle::New();
    UMyAbilityInstance* Instance = NewObject<UMyAbilityInstance>(this);
    Instance->Initialize(this, AbilityData, OutHandle);
    m_Actives.Add(OutHandle.Id, Instance);

    Instance->Activate();
    return EAbilityActivateResult::Success;
}

bool UAbilityComponent::CancelByHandle(FAbilityHandle Handle)
{
    if (auto Found = m_Actives.Find(Handle.Id))
    {
        (*Found)->End(true);
        m_Actives.Remove(Handle.Id);
        return true;
    }
    return false;
}

bool UAbilityComponent::IsOnCooldown(FGameplayTag CooldownTag) const
{
    if (const double* ExpireTime = m_CooldownsUTC.Find(CooldownTag))
    {
        return FPlatformTime::Seconds() < *ExpireTime;
    }
    return false;
}

double UAbilityComponent::GetCooldownRatio(FGameplayTag CooldownTag) const
{
	const double* ExpireTime = m_CooldownsUTC.Find(CooldownTag);
    if (*ExpireTime > 0.0)
    {
        return (FPlatformTime::Seconds() / *ExpireTime);

    }
    else
    {
        // Fail 
        return -1.0;
    }
    
}

bool UAbilityComponent::IsActive(FAbilityHandle Handle) const
{    // Actives 맵에 해당 핸들이 존재하고, 유효한 인스턴스가 있으면 true
    return m_Actives.Contains(Handle.Id);
}

void UAbilityComponent::GetActiveByTag(FGameplayTag AbilityTag, TArray<FAbilityHandle>& Out) const
{
    Out.Reset();

    for (const auto& Pair : m_Actives)
    {
        if (const UMyAbilityInstance* Instance = Pair.Value)
        {
            if (const UMyAbilityData* Data = Instance->GetAbilityData())
            {
                if (Data->m_AbilityTag == AbilityTag)
                {
                    FAbilityHandle Handle;
                    Handle.Id = Pair.Key; // Actives의 Key는 FGuid
                    Out.Add(Handle);
                }
            }
        }
    }

}

void UAbilityComponent::AddOwnedTags(const FGameplayTagContainer& Tags)
{
    m_OwnedTags.AppendTags(Tags);
}

void UAbilityComponent::RemoveOwnedTags(const FGameplayTagContainer& Tags)
{
    m_OwnedTags.RemoveTags(Tags);
}

bool UAbilityComponent::CheckOwnedTags(const FGameplayTagContainer& Tags)
{
    for(auto& Tag : Tags)
    {
        if (m_OwnedTags.HasTag(Tag) == false)
            return false;
	}
    return true;
}

FGameplayTag UAbilityComponent::CutStringToTag(FGameplayTag& FullTag)
{
    // (임시) 필요한 태그만 남기고 떼어서 쓰는 로직 
    // AbilityTag: 예) "Tag1.Tag2.Tag3"
    FString FullTagString = FullTag.ToString();

    // '.' 기준으로 분리
    TArray<FString> Segments;
    FullTagString.ParseIntoArray(Segments, TEXT("."), /*CullEmpty=*/true);

    // 마지막 세그먼트 제거
    if (Segments.Num() > 0)
    {
        Segments.RemoveAt(Segments.Num() - 1);
    }

    // 다시 '.'으로 합치기
    FString TrimmedTagString = FString::Join(Segments, TEXT("."));

    // 새로운 FGameplayTag 생성
    return FGameplayTag::RequestGameplayTag(FName(*TrimmedTagString));
}

