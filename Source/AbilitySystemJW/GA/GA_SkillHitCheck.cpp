// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_SkillHitCheck.h"
#include "AbilitySystemJW.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask/AT_WaitTargetData_Skill.h"
#include "TargetActor/TA_Skill.h"
#include "GameFramework/Character.h"
#include "CharacterComponents/TargetSystemComponent.h"
#include "Tag/JWGameplayTag.h"



UGA_SkillHitCheck::UGA_SkillHitCheck()
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

void UGA_SkillHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//JWGAS_LOG(LogJWGAS, Log, TEXT("%s"), *TriggerEventData->EventTag.GetTagName().ToString());

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	m_CurrentLevel = TriggerEventData->EventMagnitude;
	// 새로 완전히 태그들을 대체하기 위해서 추가가 아니라 대입한다. 
	m_ReceivedTags = TriggerEventData->TargetTags;

	if (m_ReceivedTags.HasTag(JWTAG_SKILL_CONFIRM_INSTANT) == true)
	{
		m_ConfirmationType = EGameplayTargetingConfirmation::Instant;
	}
	else if (m_ReceivedTags.HasTag(JWTAG_SKILL_CONFIRM_USERCONFIRM) == true)
	{
		m_ConfirmationType = EGameplayTargetingConfirmation::UserConfirmed;
	}

	ApplyAbilityTask();
}


void UGA_SkillHitCheck::OnSkillResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (SourceASC == nullptr) return;

	// 타게팅 시스템 가져오기
	UTargetSystemComponent* TargetSystemComponent = CastChecked<ACharacter>(SourceASC->GetAvatarActor())->FindComponentByClass<UTargetSystemComponent>();
	if (TargetSystemComponent == nullptr) return;


	// HitResult가 있는 경우
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (TargetASC == nullptr) return;

		// 타게팅 시스템에 등록
		TargetSystemComponent->AddTarget(TargetSystemComponent->GetTargetActor(), HitResult.GetActor());

		//Damage Effect 적용 
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(m_DamageEffect, m_CurrentLevel);
		if(SpecHandle.IsValid())
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
	// Actor만 있는 경우 ( Overlap 등 ) 
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
							// 타게팅 시스템에 등록
							TargetSystemComponent->AddTarget(TargetSystemComponent->GetTargetActor(), Actor.Get());

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

void UGA_SkillHitCheck::OnCancelledSkillCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	Super::CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,true);
}

void UGA_SkillHitCheck::ApplyAbilityTask()
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	UTargetSystemComponent* TSC = Character->FindComponentByClass<UTargetSystemComponent>();
	if (TSC == nullptr) return;

	// 타겟액터를 생성하기 위한 태스크 생성
	UAT_WaitTargetData_Skill* SkillTask = Cast< UAT_WaitTargetData_Skill>(UAT_WaitTargetData_Skill::WaitTargetData(this, FName("TAT_WaitTargetData_Skill"), m_ConfirmationType, TSC->GetSkillTargetActorClass()));
	if (SkillTask != nullptr)
	{
		SkillTask->ValidData.AddDynamic(this, &UGA_SkillHitCheck::OnSkillResultCallback);
		SkillTask->Cancelled.AddDynamic(this, &UGA_SkillHitCheck::OnCancelledSkillCallback);
		SkillTask->OnComplete.AddDynamic(this, &UGA_SkillHitCheck::OnSkillResultCallback);
		SkillTask->ReadyForActivation();
	}
}