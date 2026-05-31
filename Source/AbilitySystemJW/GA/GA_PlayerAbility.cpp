// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_PlayerAbility.h"
#include "Character/GASPlayerCharacter.h"
#include "Character/GASPlayerController.h"
#include "CharacterComponents/PlayerCombatComponent.h"
#include "Tag/JWGameplayTag.h"
#include "AbilitySystemComponent.h"


AGASPlayerCharacter* UGA_PlayerAbility::GetPlayerCharacterFromActorInfo() 
{
	if (m_CachedPlayerCharacter.IsValid() == false)
	{
		m_CachedPlayerCharacter = Cast<AGASPlayerCharacter>(CurrentActorInfo->AvatarActor);
	}
	return m_CachedPlayerCharacter.IsValid() ? m_CachedPlayerCharacter.Get() : nullptr;
}

AGASPlayerController* UGA_PlayerAbility::GetPlayerControllerFromActorInfo() 
{
	if (m_CachedPlayerController.IsValid() == false)
	{
		m_CachedPlayerController = Cast<AGASPlayerController>(CurrentActorInfo->PlayerController);
	}
	return m_CachedPlayerController.IsValid() ? m_CachedPlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UGA_PlayerAbility::GetPlayerCombatComponentFromActorInfo() 
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UGA_PlayerAbility::MakePlayerDamageEffectspecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,int32 InCurrentComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetPlayerCharacterFromActorInfo()->GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetPlayerCharacterFromActorInfo()->GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JWTAG_SHARED_SETBYCALLER_BASEDAMAGE,
		InWeaponBaseDamage);

	if(InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InCurrentComboCount);
	}

	return EffectSpecHandle;

}
