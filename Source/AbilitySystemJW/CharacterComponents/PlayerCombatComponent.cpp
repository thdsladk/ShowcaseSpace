// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/PlayerCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/JWGameplayTag.h"
#include "Items/Weapon/GASPlayerWeaponBase.h"

AGASPlayerWeaponBase* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AGASPlayerWeaponBase>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (m_OverlappedActors.Contains(HitActor))
	{
		return;
	}

	Super::OnHitTargetActor(HitActor);

	m_OverlappedActors.AddUnique(HitActor);

	//FGameplayEventData Data;
	//Data.Instigator = GetOwningPawn();
	//Data.Target = HitActor;
	//
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	//	GetOwningPawn(),
	//	JWTAG_SHARED_EVENT_MELEEDAMAGE,
	//	Data
	//);
	//
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	//	GetOwningPawn(),
	//	JWTAG_EVENT_CHARACTER_HITPAUSE,
	//	FGameplayEventData()
	//);
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	//	GetOwningPawn(),
	//	JWTAG_EVENT_CHARACTER_HITPAUSE,
	//	FGameplayEventData()
	//);
}