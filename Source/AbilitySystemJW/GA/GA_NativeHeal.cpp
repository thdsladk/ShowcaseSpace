// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_NativeHeal.h"
#include "AbilitySystemComponent.h"


UGA_NativeHeal::UGA_NativeHeal()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;


}

void UGA_NativeHeal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() && m_HealEffect)
    {
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

        // 이펙트 컨텍스트 생성
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
        Context.AddSourceObject(this);

        // 이펙트 스펙 생성
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(m_HealEffect, 1.0f, Context);

        if (SpecHandle.IsValid())
        {
            // 자기 자신에게 적용
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }


}

void UGA_NativeHeal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}
