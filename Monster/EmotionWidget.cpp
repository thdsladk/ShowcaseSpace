// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/EmotionWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UEmotionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Background->SetVisibility(ESlateVisibility::Hidden);
	Emotion->SetVisibility(ESlateVisibility::Hidden);

	// 1.0,1.0 의 크기를 가지고 있다 .
	m_Size = Emotion->RenderTransform.Scale;
}

void UEmotionWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEmotionWidget::DestroyWidget()
{
}

void UEmotionWidget::UpdateWidget()
{
}

void UEmotionWidget::StartEmotion(float Time)
{
	// Just First
	if (m_LifeCount == 0)
	{ 
		Background->SetVisibility(ESlateVisibility::Visible);
		Emotion->SetVisibility(ESlateVisibility::Visible);
		m_LifeCount = static_cast<int32>(Time*2.f);
	
		// TO DO 이미지를 변경해야한다 .
		//Emotion->Set
	}
	GetWorld()->GetTimerManager().SetTimer(m_hLifeCycle, this, &UEmotionWidget::EndEmotion, 0.5f, true);

}

void UEmotionWidget::EndEmotion()
{
	// 주의사항 중간에 취소될 경우에 대한 예외처리를 해두어야한다. 
	// Only Last
	if (m_LifeCount == 0)
	{
		Background->SetRenderScale(m_Size);
		Emotion->SetRenderScale(m_Size);

		Background->SetVisibility(ESlateVisibility::Hidden);
		Emotion->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(m_hLifeCycle);
	}
	else if (m_LifeCount > 0)
	{
		if ((m_LifeCount % 2) == 0)
		{
			Background->SetRenderScale(m_Size + FVector2D{ 0.1,0.1 });
			Emotion->SetRenderScale(m_Size + FVector2D{0.1,0.1});
		}
		else
		{
			Background->SetRenderScale(m_Size - FVector2D{ 0.1,0.1 });
			Emotion->SetRenderScale(m_Size - FVector2D{ 0.1,0.1 });
		}
		m_LifeCount--;
	}

}
