// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_HitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilityTask/AT_WeaponCollision.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "TargetActor/TA_Base.h"
#include "Attribute/CharacterAttributeSet.h"
#include "Tag/JWGameplayTag.h"
#include "Character/CharacterBase.h"
#include "Interface/PawnCombatInterface.h"
#include "CharacterComponents/PawnCombatComponent.h"
#include "Items/Weapon/GASWeaponBase.h"
#include "Components/ShapeComponent.h"


#include "AbilitySystemJW.h"

/*
*	CollisionNotify 를 통해서 시작과 종료를 제어한다.  
	HitCheck는 어빌리티가 발동하고  AbilityTask가 발동하고
	TargetActor가 충돌을 계속 넘겨주고 
	종료 이벤트가 발생하면 그때 종료가 되는 방식으로 작동한다.	
*/
UGA_HitCheck::UGA_HitCheck()
	:m_CurrentLevel(1.0f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_LIGTHHIT, nullptr);
	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_HEAVYHIT, nullptr);
	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_FLOAT, nullptr);
	m_DamageEffects.Add(JWTAG_EVENT_CHARACTER_STUN, nullptr);

}

void UGA_HitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	m_CurrentLevel = TriggerEventData->EventMagnitude;
	m_ReceivedTags = TriggerEventData->TargetTags;

	// 충돌 태스크 시작
	m_AttackWeaponTask = UAT_WeaponCollision::CreateTask(this, m_TargetActorClass);
	m_AttackWeaponTask->OnSendTargetData.AddDynamic(this, &UGA_HitCheck::OnWeaponResultCallback);
	m_AttackWeaponTask->OnComplete.AddDynamic(this, &UGA_HitCheck::EndHitCheck);
	m_AttackWeaponTask->ReadyForActivation();

	
	// 종료 이벤트 대기 태스크 시작
	UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, m_EndTag);
	WaitTask->EventReceived.AddDynamic(m_AttackWeaponTask.Get(), &UAT_WeaponCollision::FinishWeaponTask);
	WaitTask->ReadyForActivation();

}

void UGA_HitCheck::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle,ActorInfo,ActivationInfo, bReplicateCancelAbility);
	// 어빌리티가 취소되면 HitCheck를 종료한다.
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_HitCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// HitCheck가 종료되면 HitCheck를 종료한다.

	m_HitOverlappedActors.Empty();
}

void UGA_HitCheck::OnWeaponResultCallback_Implementation(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 넘어온 핸들이 없으면 어빌리티를 취소.
	if (TargetDataHandle.Num() <= 0)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}


	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (SourceASC == nullptr) return;

	// HitResult가 있는 경우
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (TargetASC == nullptr) return;

		// HitOverlappedActors에 HitResult의 Actor가 존재하면 스킵한다. 
		if (m_HitOverlappedActors.IsEmpty() == false  && m_HitOverlappedActors.Contains(HitResult.GetActor()) == true)
		{
			return;
		}
		m_HitOverlappedActors.Add(HitResult.GetActor());

		// 무적 상태일때는 데미지 폰트만 띄울수 있도록 처리하고 다른 게임 플레이 이펙트는 적용하지 않고 리턴한다. 
		//if (TargetASC->HasMatchingGameplayTag(JWTAG_CHARACTER_INVINSIBLE) == true)
		//{
		//	CastChecked<ACharacterBase>(HitResult.GetActor())->PlayDamageFont(0.f, HitResult.ImpactPoint);
		//	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		//	return;
		//}

		FGameplayEventData PayloadData;
		PayloadData.EventMagnitude = m_CurrentLevel;
		PayloadData.Instigator = SourceASC->GetAvatarActor();
		PayloadData.TargetTags = m_ReceivedTags; // 받은 태그 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), JWTAG_SHARED_EVENT_HITREACT, PayloadData);

		
		// Damage Effects 적용
		for (const FGameplayTag ReceivedTag : m_ReceivedTags)
		{
			if (m_DamageEffects.Contains(ReceivedTag) == true)
			{
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(*(m_DamageEffects.Find(ReceivedTag)), m_CurrentLevel);
				if (SpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetDataHandle);

					// Cue 실행
					FGameplayEffectContextHandle ContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(SpecHandle);
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
	
}

void UGA_HitCheck::EndHitCheck(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

