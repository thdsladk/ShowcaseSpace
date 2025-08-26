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
	// 처음 생성될 때는 보이지 않게 비활성화 되도록 한다.
	// 뷰포트에 부착이 될때 호출 되는 영역이다.
}

void UDamageFontWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Log, TEXT("Destrct DamageFont"));
}

void UDamageFontWidget::DestroyWidget(UUMGSequencePlayer& Player)
{
	// 애니메이션이 종료되면 자연스럽게 종료 하도록 여기서 바인딩을 걸어 두었다.
	// 1. 뷰에서 완전 제거
	if (IsInViewport() ==true)
		RemoveFromViewport();

	// 2. 애니 액션 취소
	CancelLatentActions();

	// 3. 애니 중지 및 해체
	StopAllAnimations();

	// 4. Slate 리소스 해제
	ReleaseSlateResources(true);

	// 5. 가시성·프로퍼티 초기화
	SetVisibility(ESlateVisibility::Collapsed);
	SetRenderOpacity(1.f);
	SetRenderTransform(FWidgetTransform()); // Translation/Scale/Shear/Angle 초기화

	// 6. 객체 풀에서 사용 가능 여부 갱신
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
	// 가시성 세팅
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
}

void UDamageFontWidget::ReleaseObject()
{
	SetIsEnabled(false);
}
