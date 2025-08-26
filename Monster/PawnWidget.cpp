// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"
#include "Character/NPCharacter.h"


void UPawnWidget::BindHp(class UMyStatComponent* StatComp)
{
	//PB_HpBar123 = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar"));
	StatComp->OnHPChanged.AddUObject(this, &UPawnWidget::UpdateHp);
	
	if (m_OwningActor.IsNull() != true)
	{
		CastChecked<ANPCharacter>(m_OwningActor)->m_ChangeAggroGauge.AddUObject(this, &UPawnWidget::UpdateAggroGauge);
	}

}

void UPawnWidget::UpdateHp(float Hp_Ratio)
{
	PB_HpBar->SetPercent(Hp_Ratio);
}

void UPawnWidget::UpdateAggroGauge(uint8 Gauge)
{
	// MaxGauge는 100이라는 전제.
	PB_AggroBar->SetPercent(static_cast<float>(Gauge) / 100.f);

}

