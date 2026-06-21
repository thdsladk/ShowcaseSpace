// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_AttackHitCheck.h"
#include "AbilitySystemJW.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask/AT_Trace.h"
#include "TargetActor/TA_Skill.h"
#include "Tag/JWGameplayTag.h"
#include "Blueprint/UserWidget.h"
#include "UI/DamageFontWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"
#include "GA_SkillHitCheck.h"

//#include "Attribute/CharacterAttributeSet.h"

UGA_AttackHitCheck::UGA_AttackHitCheck()
	:m_CurrentLevel(1.0f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	m_DamageBuffEffects.Add(JWTAG_EVENT_CHARACTER_LIGTHHIT, nullptr);
	m_DamageBuffEffects.Add(JWTAG_EVENT_CHARACTER_HEAVYHIT, nullptr);
	m_DamageBuffEffects.Add(JWTAG_EVENT_CHARACTER_FLOAT, nullptr);
	m_DamageBuffEffects.Add(JWTAG_EVENT_CHARACTER_STUN, nullptr);
	m_DamageBuffEffects.Add(JWTAG_EVENT_CHARACTER_THUNDER, nullptr);

	m_GameplayCues_Effect.Add(JWTAG_EVENT_CHARACTER_LIGTHHIT, FGameplayTag::EmptyTag);
	m_GameplayCues_Effect.Add(JWTAG_EVENT_CHARACTER_HEAVYHIT, FGameplayTag::EmptyTag);
	m_GameplayCues_Effect.Add(JWTAG_EVENT_CHARACTER_FLOAT, FGameplayTag::EmptyTag);
	m_GameplayCues_Effect.Add(JWTAG_EVENT_CHARACTER_STUN, FGameplayTag::EmptyTag);
	m_GameplayCues_Effect.Add(JWTAG_EVENT_CHARACTER_THUNDER, FGameplayTag::EmptyTag);

	m_GameplayCues_Sound.Add(JWTAG_EVENT_CHARACTER_LIGTHHIT, FGameplayTag::EmptyTag);
	m_GameplayCues_Sound.Add(JWTAG_EVENT_CHARACTER_HEAVYHIT, FGameplayTag::EmptyTag);
	m_GameplayCues_Sound.Add(JWTAG_EVENT_CHARACTER_FLOAT, FGameplayTag::EmptyTag);
	m_GameplayCues_Sound.Add(JWTAG_EVENT_CHARACTER_STUN, FGameplayTag::EmptyTag);
	m_GameplayCues_Sound.Add(JWTAG_EVENT_CHARACTER_THUNDER, FGameplayTag::EmptyTag);

}

void UGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	JWGAS_LOG(LogJWGAS, Log, TEXT("%s"), *TriggerEventData->EventTag.GetTagName().ToString());

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	m_CurrentLevel = TriggerEventData->EventMagnitude;
	// 새로 완전히 태그들을 대체하기 위해서 추가가 아니라 대입한다. 
	m_ReceivedTags = TriggerEventData->TargetTags;

	ApplyAbilityTask();
}

void UGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (SourceASC == nullptr) return;

	// HitResult가 있는 경우 ( 단일 목표 피격 )
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (TargetASC == nullptr) return;
	
		//Damage Effect 적용 
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(m_DamageEffect, m_CurrentLevel);
		if (SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetDataHandle);
		}

		// Damage Buff Effects 적용
		FGameplayEffectContextHandle ContextHandle;
		for (const FGameplayTag ReceivedTag : m_ReceivedTags)
		{
			if (m_DamageBuffEffects.Contains(ReceivedTag) == true)
			{
				FGameplayEffectSpecHandle BuffSpecHandle = MakeOutgoingGameplayEffectSpec(*(m_DamageBuffEffects.Find(ReceivedTag)), m_CurrentLevel);
				if (BuffSpecHandle.IsValid())
				{
					const TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffSpecHandle, TargetDataHandle);
					if (ActiveEffectHandles.IsEmpty() == false)
					{
						// GameplayEvent  HitReact 전달 
						FGameplayEventData PayloadData;
						PayloadData.EventMagnitude = m_CurrentLevel;
						PayloadData.Instigator = SourceASC->GetAvatarActor();
						PayloadData.TargetTags = m_ReceivedTags; // 받은 태그 전달
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), JWTAG_SHARED_EVENT_HITREACT, PayloadData);
					}


					// Cue 실행		// 여기에 있는 큐는 게임플레이 이펙트랑 독립적으로 실행되는 큐.		// 이펙트의 성공 여부로 실행되는 큐는 이펙트 안에서 설정해주고 있다. 
					ContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(BuffSpecHandle);
					ContextHandle.AddHitResult(HitResult);
					ContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
					ContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());
					BuffSpecHandle.Data->SetContext(ContextHandle);

					if (m_GameplayCues_Effect.Contains(ReceivedTag) == true)
					{
						TargetASC->ExecuteGameplayCue(*m_GameplayCues_Effect.Find(ReceivedTag), FGameplayCueParameters(ContextHandle));
					}
					if (m_GameplayCues_Sound.Contains(ReceivedTag) == true)
					{
						SourceASC->ExecuteGameplayCue(*m_GameplayCues_Sound.Find(ReceivedTag), FGameplayCueParameters(ContextHandle));
					}
				}
			}
		}
		// Buff Effects 적용
		for (auto& BuffClass : m_BuffEffects)
		{
			FGameplayEffectSpecHandle BuffSpecHandle = MakeOutgoingGameplayEffectSpec(BuffClass, m_CurrentLevel);
			if (BuffSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffSpecHandle);
			}
		}
	}
	// Actor만 있는 경우 ( 복수 목표 피격 ) 
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{

		//Damage Effect 적용 
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(m_DamageEffect, m_CurrentLevel);
		if (SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetDataHandle);
		}

		for (const FGameplayTag ReceivedTag : m_ReceivedTags)
		{
			if (m_DamageBuffEffects.Contains(ReceivedTag) == true)
			{
				FGameplayEffectSpecHandle BuffSpecHandle = MakeOutgoingGameplayEffectSpec(*(m_DamageBuffEffects.Find(ReceivedTag)), m_CurrentLevel);
				if (BuffSpecHandle.IsValid())
				{
					const TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffSpecHandle, TargetDataHandle);
					if (ActiveEffectHandles.IsEmpty() == false)
					{
						for (auto& Actor : TargetDataHandle.Data[0].Get()->GetActors())
						{
							// GameplayEvent  HitReact 전달 
							FGameplayEventData PayloadData;
							PayloadData.EventMagnitude = m_CurrentLevel;
							PayloadData.Instigator = SourceASC->GetAvatarActor();
							PayloadData.TargetTags = m_ReceivedTags; // 받은 태그 전달
							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor.Get(), JWTAG_SHARED_EVENT_HITREACT, PayloadData);
						}
					}

					FGameplayEffectContextHandle ContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(BuffSpecHandle);
					ContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
					ContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());
					BuffSpecHandle.Data->SetContext(ContextHandle);

					if (m_GameplayCues_Effect.Contains(ReceivedTag) == true)
					{
						SourceASC->ExecuteGameplayCue(*m_GameplayCues_Effect.Find(ReceivedTag), FGameplayCueParameters(ContextHandle));
					}
					if (m_GameplayCues_Sound.Contains(ReceivedTag) == true)
					{
						SourceASC->ExecuteGameplayCue(*m_GameplayCues_Sound.Find(ReceivedTag), FGameplayCueParameters(ContextHandle));
					}
				}
			}
		}
		// Buff Effects 적용
		//for (auto& BuffClass : m_BuffEffects)
		//{
		//	FGameplayEffectSpecHandle BuffSpecHandle = MakeOutgoingGameplayEffectSpec(BuffClass, m_CurrentLevel);
		//	if (BuffSpecHandle.IsValid())
		//	{
		//		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffSpecHandle);
		//	}
		//}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}


void UGA_AttackHitCheck::ApplyAbilityTask()
{
	// 타게팅 시작지점을 CreateTask 세번째 인자로 넘겨준다. 
	
	UAT_Trace* AttackTraceTask = UAT_Trace::CreateTask(this, m_TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

