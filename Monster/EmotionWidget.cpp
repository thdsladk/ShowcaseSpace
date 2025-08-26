// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/EmotionWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

#include "Data/MyGameSubsystem.h"
#include "Interface/BehaviorInterface.h"

void UEmotionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Background->SetVisibility(ESlateVisibility::Hidden);
	Emotion->SetVisibility(ESlateVisibility::Hidden);

	// 1.0,1.0 의 크기를 가지고 있다 .
	m_Size = Emotion->RenderTransform.Scale;
	m_IsPlay = false;
	m_IsScalingUp = false;
	
}

void UEmotionWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEmotionWidget::DestroyWidget()
{
}

void UEmotionWidget::UpdateCommand(uint8 Command)
{

	UMyGameSubsystem* GameSubsystem = GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	const FCommandData* CommandData = GameSubsystem->GetCommandData(Command);
	if (CommandData != nullptr)
	{
		FSlateBrush IconBrush;
		IconBrush.SetResourceObject(CommandData->Icon);
		Emotion->SetBrush(IconBrush);
	}
 
}

void UEmotionWidget::UpdateCharacterMode(uint8 Mode)
{
	ECharacterMode CharacterMode = static_cast<ECharacterMode>(Mode);
	if (CharacterMode == ECharacterMode::Battle)
	{
		StartEmotion();
	}
	else
	{
		EndEmotion();
	}
}

void UEmotionWidget::BindFunction(AActor* Owner)
{
	if (Owner != nullptr)
	{
		m_OwningActor = Owner;
		IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(Owner);
		BI->GetChangeModeDelegate().AddUObject(this, &UEmotionWidget::UpdateCharacterMode);
		BI->GetChangeCommandDelegate().AddUObject(this, &UEmotionWidget::UpdateCommand);
	}
}

void UEmotionWidget::StartEmotion()
{
	// Just First
	if (m_IsPlay == false)
	{
		m_IsPlay = true;
		Background->SetVisibility(ESlateVisibility::Visible);
		Emotion->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(m_hLifeCycle, this, &UEmotionWidget::StartEmotion, 0.5f, true);
	}
	else
	{
		if (m_IsScalingUp == false)
		{
			Background->SetRenderScale(m_Size + FVector2D{ 0.1,0.1 });
			Emotion->SetRenderScale(m_Size + FVector2D{ 0.1,0.1 });
			m_IsScalingUp = true;
		}
		else
		{
			Background->SetRenderScale(m_Size - FVector2D{ 0.1,0.1 });
			Emotion->SetRenderScale(m_Size - FVector2D{ 0.1,0.1 });
			m_IsScalingUp = false;
		}
	}

}

void UEmotionWidget::EndEmotion()
{
	// 주의사항 중간에 취소될 경우에 대한 예외처리를 해두어야한다. 
	if (m_IsPlay == true)
	{
		m_IsPlay = false;
		Background->SetRenderScale(m_Size);
		Emotion->SetRenderScale(m_Size);

		Background->SetVisibility(ESlateVisibility::Hidden);
		Emotion->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(m_hLifeCycle);
	}
}
