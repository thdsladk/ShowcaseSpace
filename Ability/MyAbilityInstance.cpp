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

    // �±� �ο�
    if (m_pAbilityData->m_Exclusivity.ApplyWhileActive.Num() > 0)
    {
        m_pAbilityComp->AddOwnedTags(m_pAbilityData->m_Exclusivity.ApplyWhileActive);
        m_AppliedTags.AppendTags(m_pAbilityData->m_Exclusivity.ApplyWhileActive);
    }

    // ���� �̸� ã��: �±��� ������ ���׸�Ʈ�� ����
    FString FullTagString = m_pAbilityData->m_AbilityTag.ToString(); 

    // '.' �������� �и�
    TArray<FString> Segments;
    FullTagString.ParseIntoArray(Segments, TEXT("."), /*CullEmpty=*/true);

    // ��Ÿ�� ���
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
    // �½�ũ ������Ʈ
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
        return; // �ߺ� ȣ�� ����
    m_bHasEnded = true;

    UE_LOG(LogTemp, Log, TEXT("Ability %s Ended (Canceled: %d)"),
        (m_pAbilityData != nullptr) ? *m_pAbilityData->m_AbilityTag.ToString() : TEXT("Unknown"), bCanceled ? 1 : 0);

    // 1) �½�ũ ����
    for (UMyAbilityTask* Task : m_Tasks)
    {
        if (Task != nullptr)
        {
            Task->Cancel();
        }
    }
    m_Tasks.Empty();

    // 2) �ο��ߴ� �±� ȸ��
    if (m_pAbilityComp != nullptr && m_AppliedTags.Num() > 0)
    {
        m_pAbilityComp->RemoveOwnedTags(m_AppliedTags);
        m_AppliedTags.Reset();
    }

    // 3) AC���� Actives ����
    if (m_pAbilityComp != nullptr)
    {
        m_pAbilityComp->CancelByHandle(m_Handle); // CancelByHandle�� ���ο��� Actives.Remove() ó��
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
