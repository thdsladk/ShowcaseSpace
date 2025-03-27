// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/EmotionWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UEmotionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Background->SetVisibility(ESlateVisibility::Hidden);
	Emotion->SetVisibility(ESlateVisibility::Hidden);

	// 1.0,1.0 �� ũ�⸦ ������ �ִ� .
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

void UEmotionWidget::StartEmotion(UTexture2D* Icon, float Time)
{

	// Just First
	if (m_LifeCount == 0)
	{
		// TO DO �̹����� �����ؾ��Ѵ� .

		FSlateBrush IconBrush;
		IconBrush.SetResourceObject(Icon);
		//IconBrush.ImageSize = FVector2D(Icon->GetSizeX(), Icon->GetSizeY());
		Emotion->SetBrush(IconBrush);

		Background->SetVisibility(ESlateVisibility::Visible);
		Emotion->SetVisibility(ESlateVisibility::Visible);
		m_LifeCount = static_cast<int32>(Time * 2.f);

	}
	GetWorld()->GetTimerManager().SetTimer(m_hLifeCycle, this, &UEmotionWidget::EndEmotion, 0.5f, true);



}

void UEmotionWidget::EndEmotion()
{
	// ���ǻ��� �߰��� ��ҵ� ��쿡 ���� ����ó���� �صξ���Ѵ�. 
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
