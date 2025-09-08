// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_PlayEffect.h"
#include "MyAbilityInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


void UAbilityTask_PlayEffect::Start(UMyAbilityInstance* Owner)
{
    Super::Start(Owner);
    m_pOwner = Owner;
    if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
    {
        Cancel();
        return;
    }
    // 타겟 액터 결정
    if (m_Target == nullptr)
    {
		m_Target = m_pOwner->GetAbilityComponent()->GetOwner();
    }




    if (m_StartDelay > 0.f)
    {
        FTimerHandle TimerHandle;
        // Delay 후 Spawn
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAbilityTask_PlayEffect::SpawnEffect, m_StartDelay, false);
    }
    else
    {
        SpawnEffect();
    }

}

void UAbilityTask_PlayEffect::Tick(float DeltaTime)
{
    if (!m_pOwner || !m_pOwner->GetAbilityComponent())
    {
        Cancel();
        return;
    }

    if (m_bEffectSpawned == true)
    {
        m_ElapsedTime += DeltaTime;
        if (m_ElapsedTime >= m_Duration)
        {
            Cancel();
        }
    }
}


void UAbilityTask_PlayEffect::Cancel()
{
    Super::Cancel();

    if (m_NiagaraComp != nullptr)
    {
        m_NiagaraComp->DestroyComponent();
    }


    m_ElapsedTime = 0.f;
    m_bEffectSpawned = false;

    m_OnCompleted.Broadcast();
	m_pOwner = nullptr;
}


bool UAbilityTask_PlayEffect::WantsTick() const
{
    // Duration이 0보다 크면 Tick 필요

    return (m_Duration > 0.f);
}



void UAbilityTask_PlayEffect::SpawnEffect()
{
    m_ElapsedTime = 0.f;
    m_bEffectSpawned = true;

    if (m_NiagaraTemplate != nullptr)
    {
        if (m_bAttachToTarget == true)
        {
            m_NiagaraComp = 
                UNiagaraFunctionLibrary::SpawnSystemAttached(
                m_NiagaraTemplate.Get(),
                m_Target->GetRootComponent(),
                m_AttachSocketName,
                m_LocationOffset,
                m_RotationOffset,
                EAttachLocation::KeepRelativeOffset,
                m_bAutoDestroy);
        }
        else
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                m_NiagaraTemplate.Get(),
                m_Target->GetActorLocation() + m_LocationOffset,
                m_RotationOffset);
        }
    }

    if (m_Sound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_Sound.Get(),
            m_Target->GetActorLocation() + m_LocationOffset);
    }

    // Duration이 0이면 즉시 완료
    if (m_Duration <= 0.f)
    {
        Cancel();
    }

}
