// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_TargetOn.h"
#include "Components/TargetSystemComponent.h"
#include "Character/CharacterBase.h"
#include "Tag/JWGameplayTag.h"
#include "AbilitySystemComponent.h"

UGA_TargetOn::UGA_TargetOn()
{
}

void UGA_TargetOn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacterBase* Character = CastChecked<ACharacterBase>(ActorInfo->AvatarActor);

	UTargetSystemComponent* TargetSystemComp = Character->GetTargetSystemComponent();
	if (TargetSystemComp != nullptr)
	{
		// uint8에 1비트필드라서 !연산자가 안먹는다. 
		TargetSystemComp->EnableAutoTargetSystem();

		if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(JWTAG_CHARACTER_ISTARGETING) == true)
		{
			ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(JWTAG_CHARACTER_ISTARGETING);
		}
		else
		{
			ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(JWTAG_CHARACTER_ISTARGETING);
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	}

}

