// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/PawnCombatComponent.h"
#include "Items/Weapon/GASWeaponBase.h"

#include "Components/BoxComponent.h"

#include "Header/JWHeader.h"


void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AGASWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(m_CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister) == false, TEXT("A named named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	m_CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	//InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTargetActor);
	//InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &UPawnCombatComponent::OnWeaponPulledFromTargetActor);

	if (bRegisterAsEquippedWeapon)
	{
		//m_CurrentEquippedWeaponTag = InWeaponTagToRegister;
		// 추가하는 방식
		m_CurrentEquippedWeaponTags.Add(InWeaponToRegister->GetEquipmentType(), InWeaponTagToRegister);
	}

	//const FString WeaponString = FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	//Debug::Print(WeaponString);
}

AGASWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (m_CharacterCarriedWeaponMap.Contains(InWeaponTagToGet) == true)
	{
		if (AGASWeaponBase* const* FoundWeapon = m_CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AGASWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon(EEquipmentType Type) const
{
	if (m_CurrentEquippedWeaponTags[Type].IsValid() == false)
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(m_CurrentEquippedWeaponTags[Type]);
}

TArray<AGASWeaponBase*> UPawnCombatComponent::GetCharacterAllCurrentEquippedWeapons() const
{
	if (m_CurrentEquippedWeaponTags.IsEmpty() == true)
	{
		return TArray<AGASWeaponBase*>();
	}
	TArray<AGASWeaponBase*> WeaponList;
	
	for (auto& Pair : m_CurrentEquippedWeaponTags)
	{
		WeaponList.Add(GetCharacterCarriedWeaponByTag(Pair.Value));
	}
	return WeaponList;
}

void UPawnCombatComponent::AddCurrentEquippedWeapon(EEquipmentType Type, FGameplayTag InWeaponTagToRegister)
{
	m_CurrentEquippedWeaponTags.Add(Type, InWeaponTagToRegister);
}



void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else
	{
		ToggleBodyCollsionBoxCollision(bShouldEnable, ToggleDamageType);
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Debug::Print(TEXT("Target On"), FColor::Blue);
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	TArray<AGASWeaponBase*> WeaponToToggles = GetCharacterAllCurrentEquippedWeapons();
	for (auto WeaponToToggle : WeaponToToggles)
	{
		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			m_OverlappedActors.Empty();
		}
	}
}

void UPawnCombatComponent::ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
}
