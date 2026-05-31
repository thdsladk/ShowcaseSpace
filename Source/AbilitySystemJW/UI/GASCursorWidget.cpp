// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASCursorWidget.h"
#include "GASCursorWidget.h"
#include "Interface\PlayerControllerInterface.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/WidgetLayoutLibrary.h"


UGASCursorWidget::UGASCursorWidget()
	:m_CurrentCursorPos(FVector2D::ZeroVector)
{

}

void UGASCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APlayerController* PC = GetOwningPlayer())
	{
		// 헤더에 변수 추가

		FVector2D MousePos;

		// NativeTick
		if (PC->GetMousePosition(MousePos.X, MousePos.Y))
		{
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(PC);
			MousePos /= ViewportScale;

			// 보간 (저장된 위치 기준)
			m_CurrentCursorPos = FMath::Vector2DInterpTo(m_CurrentCursorPos, MousePos, InDeltaTime, 20.f);

			SetPositionInViewport(m_CurrentCursorPos, false);
		}
	}
}

void UGASCursorWidget::InitWidget()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(PC))
	{
		SetOwningPlayer(GetWorld()->GetFirstPlayerController());
		PCI->OnChangeCombatMode().AddUObject(this, &UGASCursorWidget::UpdateCombatMode);
	}
}

void UGASCursorWidget::UpdateCombatMode()
{
	if (IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(GetWorld()->GetFirstPlayerController()))
	{
		if (WidgetSwitcher != nullptr)
		{
			WidgetSwitcher->SetActiveWidgetIndex(PCI->GetCombatMode());
		}
	}
}
