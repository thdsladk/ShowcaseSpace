// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GaugeBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/CharacterAttributeSet.h"
#include "Header/JWGameplayTag.h"


void UGaugeBarWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (m_AbilitySystemComponent != nullptr)
	{
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetChargeGaugeAttribute()).AddUObject(this, &UGaugeBarWidget::OnGaugeChanged);
		m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxChargeGaugeAttribute()).AddUObject(this, &UGaugeBarWidget::OnMaxGaugehanged);
		//GaugeBar->SetFillColorAndOpacity(mGaugeColor);



		const UCharacterAttributeSet* CurrentAttributeSet = m_AbilitySystemComponent->GetSet<UCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			m_CurrentGauge = CurrentAttributeSet->GetChargeGauge();
			m_CurrentMaxGauge = CurrentAttributeSet->GetMaxChargeGauge();


			if (m_CurrentMaxGauge > 0.0f)
			{
				UpdateGaugeBar();
			}

		}
	}
}

void UGaugeBarWidget::OnGaugeChanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentGauge = FMath::Clamp(ChangeData.NewValue, 0.f, 1.f);
	UpdateGaugeBar();
}

void UGaugeBarWidget::OnMaxGaugehanged(const FOnAttributeChangeData& ChangeData)
{
	m_CurrentMaxGauge = FMath::Clamp(ChangeData.NewValue, 0.f, 1.f);
	UpdateGaugeBar();
}

void UGaugeBarWidget::UpdateGaugeBar()
{
	float MaxPercent = 1.0f;
	if (GaugeBar)
	{	
		GaugeBar->SetPercent(MaxPercent - (m_CurrentGauge / m_CurrentMaxGauge));
	}
	if (GaugeText)
	{
		FString GaugeString = FString::Printf(TEXT("%.0f / %.0f"), m_CurrentGauge, m_CurrentMaxGauge);
		GaugeText->SetText(FText::FromString(GaugeString));
	}
}

void UGaugeBarWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{

}