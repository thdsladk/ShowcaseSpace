// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JWGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "Attribute/CharacterSkillAttributeSet.h"

AJWGASPlayerState::AJWGASPlayerState()
{
	m_AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	m_AbilitySystemComponent->SetIsReplicated(true);
	m_AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
	m_SkillAttributeSet = CreateDefaultSubobject<UCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));

}

UAbilitySystemComponent* AJWGASPlayerState::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

