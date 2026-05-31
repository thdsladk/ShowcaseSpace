// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASGaugeBarUserWidget.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "Tag/JWGameplayTag.h"

UGASGaugeBarUserWidget::UGASGaugeBarUserWidget()
	:m_VisibleHP(true)
	,m_VisibleMP(true)
	,m_VisibleSP(true)
{
	
}

void UGASGaugeBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (m_AbilitySystemComponent != nullptr)
	{
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UGASGaugeBarUserWidget::OnHPChanged);
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UGASGaugeBarUserWidget::OnMaxHPChanged);
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetManaAttribute()).AddUObject(this, &UGASGaugeBarUserWidget::OnMPChanged);
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UGASGaugeBarUserWidget::OnMaxMPChanged);
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute()).AddUObject(this, &UGASGaugeBarUserWidget::OnSPChanged);
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &UGASGaugeBarUserWidget::OnMaxSPChanged);
		m_AbilitySystemComponent->RegisterGameplayTagEvent(JWTAG_CHARACTER_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASGaugeBarUserWidget::OnInvinsibleTagChanged);
		//HpBar->SetFillColorAndOpacity(m_HPColor);
		//SpBar->SetFillColorAndOpacity(m_HPColor);
		//MpBar->SetFillColorAndOpacity(m_HPColor);
		(m_VisibleHP == true) ? OverlayHP->SetVisibility(ESlateVisibility::SelfHitTestInvisible) : OverlayHP->SetVisibility(ESlateVisibility::Hidden);
		(m_VisibleMP == true) ? OverlayMP->SetVisibility(ESlateVisibility::SelfHitTestInvisible) : OverlayMP->SetVisibility(ESlateVisibility::Hidden);
		(m_VisibleSP == true) ? OverlaySP->SetVisibility(ESlateVisibility::SelfHitTestInvisible) : OverlaySP->SetVisibility(ESlateVisibility::Hidden);


		const UCharacterAttributeSet* CurrentAttributeSet = m_AbilitySystemComponent->GetSet<UCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			m_CurrentHP = CurrentAttributeSet->GetHealth();
			m_CurrentMaxHP = CurrentAttributeSet->GetMaxHealth();
			m_CurrentMP = CurrentAttributeSet->GetMana();
			m_CurrentMaxMP = CurrentAttributeSet->GetMaxMana();
			m_CurrentSP = CurrentAttributeSet->GetStamina();
			m_CurrentMaxSP = CurrentAttributeSet->GetMaxStamina();

			if (m_CurrentMaxHP > 0.0f)
			{
				UpdateHpBar();
			}
			if (m_CurrentMaxMP > 0.0f)
			{
				UpdateMpBar();
			}
			if (m_CurrentMaxSP > 0.0f)
			{
				UpdateSpBar();
			}
		}
	}
}

void UGASGaugeBarUserWidget::OnHPChanged(const FOnAttributeChangeData& ChangeData)
{

	m_CurrentHP = ChangeData.NewValue;
	UpdateHpBar();
}

void UGASGaugeBarUserWidget::OnMaxHPChanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentMaxHP = ChangeData.NewValue;
	UpdateHpBar();
}

void UGASGaugeBarUserWidget::OnMPChanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentMP = ChangeData.NewValue;
	UpdateMpBar();
}

void UGASGaugeBarUserWidget::OnMaxMPChanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentMaxMP = ChangeData.NewValue;
	UpdateMpBar();
}

void UGASGaugeBarUserWidget::OnSPChanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentSP = ChangeData.NewValue;
	UpdateSpBar();
}

void UGASGaugeBarUserWidget::OnMaxSPChanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentMaxSP = ChangeData.NewValue;
	UpdateSpBar();
}

void UGASGaugeBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		HpBar->SetFillColorAndOpacity(m_InvinsibleColor);
		HpBar->SetPercent(0.0f);
	}
	else
	{
		HpBar->SetFillColorAndOpacity(m_HPColor);
		UpdateHpBar();
	}
}

void UGASGaugeBarUserWidget::UpdateHpBar()
{
	const float MaxPercent = 1.0f;

	if (HpBar != nullptr)
	{
		HpBar->SetPercent(MaxPercent - (m_CurrentHP / m_CurrentMaxHP));
	}

	if (HpText != nullptr)
	{
		HpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), m_CurrentHP, m_CurrentMaxHP)));
	}
}

void UGASGaugeBarUserWidget::UpdateMpBar()
{
	const float MaxPercent = 1.0f;
	if (MpBar != nullptr)
	{
		MpBar->SetPercent(MaxPercent - (m_CurrentMP / m_CurrentMaxMP));
	}

	if (MpText != nullptr)
	{
		MpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), m_CurrentMP, m_CurrentMaxMP)));
	}
}

void UGASGaugeBarUserWidget::UpdateSpBar()
{
	const float MaxPercent = 1.0f;
	if (SpBar != nullptr)
	{
		SpBar->SetPercent(MaxPercent - (m_CurrentSP / m_CurrentMaxSP));
	}

	if (SpText != nullptr)
	{
		SpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), m_CurrentSP, m_CurrentMaxSP)));
	}
}
