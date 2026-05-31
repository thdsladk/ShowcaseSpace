// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASUserWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if(IsValid(InOwner))
	{
		m_AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
	else
	{
		m_AbilitySystemComponent = nullptr;
	}
}

UAbilitySystemComponent* UGASUserWidget::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}
