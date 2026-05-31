// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotBaseWidget.h"

USlotBaseWidget::USlotBaseWidget()
	: m_CooldownTime(0.f)
{
}

void USlotBaseWidget::PlayGaugeAnimation()
{
	PlayAnimation(PlayGauge, 0.0f,1,EUMGSequencePlayMode::Forward, (1.f/m_CooldownTime));
}
