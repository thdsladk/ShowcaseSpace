// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DamageFontWidget.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "TimerManager.h"
#include "Engine/World.h"

UDamageFontWidget::UDamageFontWidget()
	: m_IsPlay(false)
	, m_IsMissed(false)
{
}

void UDamageFontWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	SetVisibility(ESlateVisibility::Hidden);
	
}

void UDamageFontWidget::InitializeDamageFont(float InDamage) 
{
	if (FMath::IsNearlyZero(InDamage) == true)
	{
		m_IsMissed = true;

	}

	m_CurrentDamage = InDamage; 
}

void UDamageFontWidget::PlayDamageFont()
{
	if (m_IsPlay == false && Upper != nullptr)
	{
		SetVisibility(ESlateVisibility::Visible);

		UpdateTextBlock();
		
		PlayAnimation(Upper);
		m_IsPlay = true;
		
	}

}

void UDamageFontWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 애니메이션이 있으면 완료 바인딩
	if (Upper != nullptr)
	{
		FWidgetAnimationDynamicEvent FinishedEvent;
		FinishedEvent.BindDynamic(this, &UDamageFontWidget::OnLifeTimeEnded);
		BindToAnimationFinished(Upper, FinishedEvent);
	}
	else
	{
		OnLifeTimeEnded();
	}
}

void UDamageFontWidget::NativeDestruct()
{
	// 안전장치: 파괴 시에도 델리게이트/타이머 정리
	CleanupAndRemove();

	Super::NativeDestruct();
}

void UDamageFontWidget::UpdateTextBlock()
{
	if (DamageText != nullptr)
	{
		if (m_IsMissed == true)
		{
			DamageText->SetText(FText::FromString(TEXT(" Miss ")));
			DamageText->SetColorAndOpacity(m_MissColor);
		}
		else
		{
			FString GaugeString = FString::Printf(TEXT(" %.0f "), m_CurrentDamage);
			DamageText->SetText(FText::FromString(GaugeString));
			DamageText->SetColorAndOpacity(m_NormalColor);
			//m_CurrentDamage = 0.f;
		}
	}
}

void UDamageFontWidget::OnLifeTimeEnded()
{
	// 타임아웃으로 호출되면 정리
	CleanupAndRemove();
}

void UDamageFontWidget::CleanupAndRemove()
{
	// 실제 위젯 제거
	RemoveFromParent();
}