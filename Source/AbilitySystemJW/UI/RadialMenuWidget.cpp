// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RadialMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Header/GASStructTypes.h"


URadialMenuWidget::URadialMenuWidget()
	: m_IsActive(false)
	, m_IsOpening(false)
	, m_IsClosing(false)
{
    SetVisibility(ESlateVisibility::Hidden);

}

void URadialMenuWidget::PlayRadialMenu()
{
    if (MainPanel == nullptr) return;

	m_IsActive = true;
    SetVisibility(ESlateVisibility::Visible);


    // 기존 자식들을 가져오기
    const TArray<UWidget*>& Children = MainPanel->GetAllChildren();
    
    m_TargetPositions.Empty();

    for (int32 i = 0; i < Children.Num(); ++i)
    {
        UWidget* Child = Children[i];
        if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Child->Slot))
        {
            // 초기 위치는 원점
            PanelSlot->SetPosition(FVector2D(0.f, 0.f));
            PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            PanelSlot->SetAutoSize(true);

            // 목표 위치 계산
            float Angle = (2.f * PI / Children.Num()) * i;
            FVector2D TargetPos(m_Radius * FMath::Cos(Angle), m_Radius * FMath::Sin(Angle));
            m_TargetPositions.Add(TargetPos);
        }
    }

	m_IsOpening = true;

    // 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(m_MenuTickHandle, this, &URadialMenuWidget::OpeningTick, m_TickInterval, true);

}

void URadialMenuWidget::EndRadialMenu()
{
    // 활성화가 되어있어야 작동.
	if (m_IsActive == false) return;

	m_IsClosing = true;

    // 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(m_MenuTickHandle, this, &URadialMenuWidget::ClosingTick, m_TickInterval, true);
}

void URadialMenuWidget::MenuTick()
{    
}

void URadialMenuWidget::OpeningTick()
{
    m_CurrentPercent = FMath::Clamp(m_CurrentPercent + (m_TickInterval*m_PlaySpeed), 0.f, 1.f);
    float Value = 0.0f;
    float FadeInValue = 0.0f;
    if (m_CurveTable)
    {
        FRealCurve* Curve = m_CurveTable->FindCurve(FName(TEXT("Curve")), TEXT("Curve"));
        if (Curve != nullptr)
        {
            Value = Curve->Eval(m_CurrentPercent); // Y값 반환
        }
        FRealCurve* FadeInCurve = m_CurveTable->FindCurve(FName(TEXT("FadeIn")), TEXT("FadeIn"));
        if (FadeInCurve != nullptr)
        {
			FadeInValue = FadeInCurve->Eval(m_CurrentPercent); // Y값 반환
        }


    }

    const TArray<UWidget*>& Children = MainPanel->GetAllChildren();
    for (int32 i = 0; i < Children.Num(); ++i)
    {
        if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Children[i]->Slot))
        {
            FVector2D NewPos = FMath::Lerp(FVector2D::ZeroVector, m_TargetPositions[i], Value);
            PanelSlot->SetPosition(NewPos);
            Children[i]->SetRenderOpacity(FadeInValue);
        }
    }

    if (Value >= 1.0f)
    {
        m_IsOpening = false;
        GetWorld()->GetTimerManager().ClearTimer(m_MenuTickHandle);
    }

}

void URadialMenuWidget::ClosingTick()
{
    m_CurrentPercent = FMath::Clamp(m_CurrentPercent - (m_TickInterval * m_PlaySpeed), 0.f, 1.f);

    float Value = 0.0f;
    float FadeOutValue = 0.0f;
    if (m_CurveTable)
    {
        FRealCurve* Curve = m_CurveTable->FindCurve(FName(TEXT("InverseCurve")), TEXT("InverseCurve"));
        if (Curve != nullptr)
        {
            Value = Curve->Eval(m_CurrentPercent); // Y값 반환
        }
    }
    FRealCurve* FadeOutCurve = m_CurveTable->FindCurve(FName(TEXT("FadeOut")), TEXT("FadeOut"));
    if (FadeOutCurve != nullptr)
    {
        FadeOutValue = FadeOutCurve->Eval(m_CurrentPercent); // Y값 반환
    }

    const TArray<UWidget*>& Children = MainPanel->GetAllChildren();
    for (int32 i = Children.Num() - 1; i >= 0; --i)
    {
        if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Children[i]->Slot))
        {
            FVector2D NewPos = FMath::Lerp(FVector2D::ZeroVector, m_TargetPositions[i], Value);
            PanelSlot->SetPosition(NewPos);
            Children[i]->SetRenderOpacity(FadeOutValue);
        }
    }

    if (Value <= 0.0f)
    {
        m_IsClosing = false;
        m_IsActive = false;
        SetVisibility(ESlateVisibility::Hidden);
        GetWorld()->GetTimerManager().ClearTimer(m_MenuTickHandle);
    }
}


