// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Sprint.h"
#include "Interface/MoveInterface.h"
#include "Interface/PlayerControllerInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"

UGA_Sprint::UGA_Sprint() : m_CostTimerHandle(), m_Interval(1.f), m_CostPerInterval(0.f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
	{
		return false;
	}
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC != nullptr)
	{
		const UCharacterAttributeSet* Attribute = ASC->GetSet<UCharacterAttributeSet>();
		if (Attribute != nullptr)
		{
			if (Attribute->GetStamina() >= m_CostPerInterval)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Super::CommitAbility(Handle, ActorInfo, ActivationInfo);


	IMoveInterface* MoveInterface = CastChecked<IMoveInterface>(ActorInfo->AvatarActor.Get());
	MoveInterface->StartSprint();

	GetWorld()->GetTimerManager().SetTimer(m_CostTimerHandle, this, &UGA_Sprint::CostTimer, m_Interval, true);
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	IMoveInterface* MoveInterface = CastChecked<IMoveInterface>(ActorInfo->AvatarActor.Get());
	MoveInterface->StopSprint();


	GetWorld()->GetTimerManager().ClearTimer(m_CostTimerHandle);
}

void UGA_Sprint::CostTimer()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (ASC != nullptr)
	{
		const UCharacterAttributeSet* Attribute = ASC->GetSet<UCharacterAttributeSet>();
		if(Attribute != nullptr)
		{
			const float CurrentStamina = Attribute->GetStamina();
			if (CurrentStamina < m_CostPerInterval)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}
			else
			{
				ASC->ApplyModToAttributeUnsafe(UCharacterAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -m_CostPerInterval);
			}
		}
		else
		{
			// 비정상 종료. 그냥 터트리는게 더 좋을 수도 있다.
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}



}