// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageFontWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Overlay.h"

UDamageFontWidget::UDamageFontWidget()
{
	SetIsEnabled(false);


}

void UDamageFontWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// ó�� ������ ���� ������ �ʰ� ��Ȱ��ȭ �ǵ��� �Ѵ�.
	// ����Ʈ�� ������ �ɶ� ȣ�� �Ǵ� �����̴�.
}

void UDamageFontWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Log, TEXT("Destrct DamageFont"));
}

void UDamageFontWidget::DestroyWidget(UUMGSequencePlayer& Player)
{
	// �ִϸ��̼��� ����Ǹ� �ڿ������� ���� �ϵ��� ���⼭ ���ε��� �ɾ� �ξ���.
	// 1. �信�� ���� ����
	if (IsInViewport() ==true)
		RemoveFromViewport();

	// 2. �ִ� �׼� ���
	CancelLatentActions();

	// 3. �ִ� ���� �� ��ü
	StopAllAnimations();

	// 4. Slate ���ҽ� ����
	ReleaseSlateResources(true);

	// 5. ���ü���������Ƽ �ʱ�ȭ
	SetVisibility(ESlateVisibility::Collapsed);
	SetRenderOpacity(1.f);
	SetRenderTransform(FWidgetTransform()); // Translation/Scale/Shear/Angle �ʱ�ȭ

	// 6. ��ü Ǯ���� ��� ���� ���� ����
	ReleaseObject();
}

void UDamageFontWidget::UpdateWidget()
{
	

}

void UDamageFontWidget::TakeDamage(int32 Value, EHitType HitType)
{
	UWidgetAnimation* Anim = nullptr;
	if (HitType == EHitType::HitNormal)
	{
		
		Anim = Anim_Normal;

	}
	
	if (Anim == nullptr)return;

	UTextBlock* Normal = Cast<UTextBlock>(Overlay_Text->GetChildAt(0));
	if (Normal == nullptr)return;

	if (Value == 0)
	{
		Normal->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		Normal->SetText(FText::FromString(TEXT("MISS")));

		
		UUMGSequencePlayer* Player = PlayAnimationForward(Anim);
		if (Player)
		{
			Player->OnSequenceFinishedPlaying().AddUObject(
				this, &UDamageFontWidget::DestroyWidget);
		}

	}
	else if(Value > 0)
	{
		
		Normal->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		Normal->SetText(FText::FromString(FString::FromInt(Value)));

		UUMGSequencePlayer* Player = PlayAnimationForward(Anim,1.0f,true);
		if (Player)
		{
			Player->OnSequenceFinishedPlaying().AddUObject(
				this, &UDamageFontWidget::DestroyWidget
			);
		}
	}
	else if (Value < 0)
	{
		Normal->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		Normal->SetText(FText::FromString(FString::FromInt(Value)));

		UUMGSequencePlayer* Player = PlayAnimationForward(Anim, 1.0f, true);
		if (Player)
		{
			Player->OnSequenceFinishedPlaying().AddUObject(
				this, &UDamageFontWidget::DestroyWidget
			);
		}
	}
	
}

bool UDamageFontWidget::IsUseableObject()
{
	return !GetIsEnabled();
}

void UDamageFontWidget::ApplyHidden(bool bHide)
{
	//SetIsEnabled(!bHide);
}

void UDamageFontWidget::AcquireObject()
{
	// ���ü� ����
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
}

void UDamageFontWidget::ReleaseObject()
{
	SetIsEnabled(false);
}
