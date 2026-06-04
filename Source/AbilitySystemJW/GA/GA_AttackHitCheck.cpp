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

	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_LIGTHHIT, nullptr);
	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_HEAVYHIT, nullptr);
	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_FLOAT, nullptr);
	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_STUN, nullptr);

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

	// HitResult가 있는 경우
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (TargetASC == nullptr) return;

		// 무적 상태일때는 데미지 폰트만 띄울수 있도록 처리하고 다른 게임 플레이 이펙트는 적용하지 않고 리턴한다. 
		//if (TargetASC->HasMatchingGameplayTag(JWTAG_CHARACTER_INVINSIBLE) == true)
		//{
		//	CastChecked<ACharacterBase>(HitResult.GetActor())->PlayDamageFont(0.f, HitResult.ImpactPoint);
		//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		//	return;
		//}

		FGameplayEventData PayloadData;
		PayloadData.EventMagnitude = m_CurrentLevel;
		PayloadData.Instigator = SourceASC->GetAvatarActor();
		PayloadData.TargetTags = m_ReceivedTags; // 받은 태그 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), JWTAG_SHARED_EVENT_HITREACT, PayloadData);
	
		// Damage Effects 적용
		FGameplayEffectContextHandle ContextHandle;
		for (const FGameplayTag ReceivedTag : m_ReceivedTags)
		{
			if (m_DamageEffects.Contains(ReceivedTag) == true)
			{
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(*(m_DamageEffects.Find(ReceivedTag)), m_CurrentLevel);
				if (SpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetDataHandle);

					// Cue 실행		// 여기에 있는 큐는 게임플레이 이펙트랑 독립적으로 실행되는 큐.		// 이펙트의 성공 여부로 실행되는 큐는 이펙트 안에서 설정해주고 있다. 
						ContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(SpecHandle);
						ContextHandle.AddHitResult(HitResult);
						ContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);

						for (auto& CueTag : m_GameplayCues)
						{
							TargetASC->ExecuteGameplayCue(CueTag, FGameplayCueParameters(ContextHandle));
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
	// Actor만 있는 경우
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		for (const FGameplayTag ReceivedTag : m_ReceivedTags)
		{
			if (m_DamageEffects.Contains(ReceivedTag) == true)
			{
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(*(m_DamageEffects.Find(ReceivedTag)), m_CurrentLevel);
				if (SpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetDataHandle);

					FGameplayEffectContextHandle ContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(SpecHandle);
					ContextHandle.AddActors(TargetDataHandle.Data[0]->GetActors(), false);

					for (auto& CueTag : m_GameplayCues)
					{
						SourceASC->ExecuteGameplayCue(CueTag, FGameplayCueParameters(ContextHandle));
					}
				}
			}
		}

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

