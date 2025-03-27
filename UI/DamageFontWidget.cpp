// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageFontWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UDamageFontWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(m_hLifeCycle,this,&UDamageFontWidget::NativeDestruct, 4.f, false);
	
}

void UDamageFontWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(m_hLifeCycle);
}

void UDamageFontWidget::DestroyWidget()
{

}

void UDamageFontWidget::UpdateWidget()
{
	

}

void UDamageFontWidget::TakeDamage(int32 Value)
{
	if (Value == 0)
	{
		Text_Normal->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		Text_Normal->SetText(FText::FromString(TEXT("MISS")));
	}
	else if(Value > 0)
	{
		Text_Normal->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		Text_Normal->SetText(FText::FromString(FString::FromInt(Value)));
	}
	else if (Value < 0)
	{
		Text_Normal->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
		Text_Normal->SetText(FText::FromString(FString::FromInt(Value)));
	}
	
}
