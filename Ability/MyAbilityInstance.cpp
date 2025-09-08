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
    // 1) 필수 포인터 검사
    if (m_pAbilityComp == nullptr || m_pAbilityData == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Activate failed: Missing AC or Data"));
        return EAbilityActivateResult::InternalError;
    }

    // 2) 발동 중 태그 부여
    if (m_pAbilityData->m_Exclusivity.ApplyWhileActive.Num() > 0)
    {
        m_pAbilityComp->AddOwnedTags(m_pAbilityData->m_Exclusivity.ApplyWhileActive);
        m_AppliedTags.AppendTags(m_pAbilityData->m_Exclusivity.ApplyWhileActive);
    }

    // 3) AbilityTag에서 마지막 세그먼트를 섹션 이름으로 추출
    FString FullTagString = m_pAbilityData->m_AbilityTag.ToString();  
    TArray<FString> Segments;
    FullTagString.ParseIntoArray(Segments, TEXT("."), /*CullEmpty=*/true);

    // 4) 몽타주 재생
    if (m_pAbilityData->m_pMontage != nullptr && Segments.Num() > 0)
    {
        const FName SectionName(*Segments.Last()); 

        if (USkeletalMeshComponent* MeshComp = m_pAbilityComp->GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
        {
            if (UAnimInstance* Anim = MeshComp->GetAnimInstance())
            {
                Anim->Montage_Play(m_pAbilityData->m_pMontage);
                Anim->Montage_JumpToSection(SectionName, m_pAbilityData->m_pMontage);

                UE_LOG(LogTemp, Log, TEXT("Playing Montage Section: %s"), *SectionName.ToString());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Montage not set or invalid AbilityTag format"));
    }

    // 5) 태스크 파이프라인 실행
    if (m_pAbilityData->m_TaskPipeline.Num() > 0)
    {
        for (TSubclassOf<UMyAbilityTask> TaskClass : m_pAbilityData->m_TaskPipeline)
        {
            if (TaskClass != nullptr)
            {
                // 태스크 생성 (Outer를 Instance로 지정)
                UMyAbilityTask* Task = NewObject<UMyAbilityTask>(this, TaskClass);
                m_Tasks.Add(Task);

                // 태스크 시작
                Task->Start(this);

                UE_LOG(LogTemp, Log, TEXT("Started Task: %s"), *TaskClass->GetName());
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
        if (Task != nullptr && Task->WantsTick() ==true)
        {
            Task->Tick(DeltaTime);
        }
    }
    
}

void UMyAbilityInstance::End(bool bCanceled /*,FGameplayTag Reason*/)
{
    // bCanceled로 취소된건지 종료인건지 구분  
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
