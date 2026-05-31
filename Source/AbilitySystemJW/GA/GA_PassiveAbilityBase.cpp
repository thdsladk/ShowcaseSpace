// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_PassiveAbilityBase.h"
#include "AbilitySystemComponent.h"

void UGA_PassiveAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Spec.Handle))
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
	}
}
