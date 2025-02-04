// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnWidget.h"
#include "MonsterStatComponent.h"
#include "Components/ProgressBar.h"


void UPawnWidget::BindHp(class UMonsterStatComponent* StatComp)
{
	//PB_HpBar123 = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar"));
	CurrentStatComp = StatComp;
	StatComp->OnHPChanged.AddUObject(this, &UPawnWidget::UpdateHp);
}

void UPawnWidget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetHPRatio());
}
