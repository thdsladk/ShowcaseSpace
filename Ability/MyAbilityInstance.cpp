// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilityInstance.h"
#include "MyAbilityTask.h"
#include "Animation\AnimInstance.h"

void UMyAbilityInstance::Initialize(UAbilityComponent* pAbilityComp,UMyAbilityData* pData, FAbilityHandle Handle)
{
	m_pAbilityComp = pAbilityComp;
	m_pAbilityData = pData;
	m_Handle = Handle;

    m_bHasEnded = false;

}

EAbilityActivateResult UMyAbilityInstance::Activate()
{
    if (m_pAbilityComp == nullptr || m_pAbilityData == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Activate failed: Missing AC or Data"));
        return EAbilityActivateResult::InternalError;
    }

    // 태그 부여
    if (m_pAbilityData->m_Exclusivity.ApplyWhileActive.Num() > 0)
    {
        m_pAbilityComp->AddOwnedTags(m_pAbilityData->m_Exclusivity.ApplyWhileActive);
        m_AppliedTags.AppendTags(m_pAbilityData->m_Exclusivity.ApplyWhileActive);
    }

    // 섹션 이름 찾기: 태그의 마지막 세그먼트를 추출
    FString FullTagString = m_pAbilityData->m_AbilityTag.ToString(); 

    // '.' 기준으로 분리
    TArray<FString> Segments;
    FullTagString.ParseIntoArray(Segments, TEXT("."), /*CullEmpty=*/true);

    // 몽타주 재생
    if (Segments.IsEmpty() == false)
    {
        USkeletalMeshComponent* MeshComp = m_pAbilityComp->GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
        if (MeshComp != nullptr)
        {
            UAnimInstance* Anim = Cast<UAnimInstance>(MeshComp->GetAnimInstance());
            if (Anim != nullptr)
            {
				Anim->Montage_Play(m_pAbilityData->m_pMontage);
                Anim->Montage_JumpToSection(*Segments.Last(), m_pAbilityData->m_pMontage);

                UE_LOG(LogTemp, Log, TEXT("Playing Montage Section: %s"), *Segments.Last());
            }
        }
    }

    return EAbilityActivateResult::Success;

}

void UMyAbilityInstance::Tick(float DeltaTime) 
{
    // 태스크 업데이트
    for (UMyAbilityTask* Task : m_Tasks)
    {
        if (Task != nullptr && Task->WantsTick())
        {
            Task->Tick(DeltaTime);
        }
    }
}

void UMyAbilityInstance::End(bool bCanceled /*,FGameplayTag Reason*/)
{
    if (m_bHasEnded == true)
        return; // 중복 호출 방지
    m_bHasEnded = true;

    UE_LOG(LogTemp, Log, TEXT("Ability %s Ended (Canceled: %d)"),
        (m_pAbilityData != nullptr) ? *m_pAbilityData->m_AbilityTag.ToString() : TEXT("Unknown"), bCanceled ? 1 : 0);

    // 1) 태스크 정리
    for (UMyAbilityTask* Task : m_Tasks)
    {
        if (Task != nullptr)
        {
            Task->Cancel();
        }
    }
    m_Tasks.Empty();

    // 2) 부여했던 태그 회수
    if (m_pAbilityComp != nullptr && m_AppliedTags.Num() > 0)
    {
        m_pAbilityComp->RemoveOwnedTags(m_AppliedTags);
        m_AppliedTags.Reset();
    }

    // 3) AC에서 Actives 제거
    if (m_pAbilityComp != nullptr)
    {
        m_pAbilityComp->CancelByHandle(m_Handle); // CancelByHandle이 내부에서 Actives.Remove() 처리
    }

}

bool UMyAbilityInstance::IsCancelable() const
{
    if (m_bHasEnded == true || m_pAbilityData == nullptr)
    {
        return false;
    }
    else
    {
        return m_pAbilityData->m_bCancelable;
    }

}
