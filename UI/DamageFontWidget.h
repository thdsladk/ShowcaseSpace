// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "Header/GlobalEnum.h"
#include "Interface/PoolableInterface.h"
#include "DamageFontWidget.generated.h"

class UWidgetAnimation;
class UTextBlock;
class UOverlay;
/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UDamageFontWidget : public UMyUserWidget, public IPoolableInterface
{
	GENERATED_BODY()
public:
	UDamageFontWidget();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void DestroyWidget(UUMGSequencePlayer& Player);

	UFUNCTION()
	void UpdateWidget();

	void TakeDamage(int32 Value, EHitType HitType = EHitType::HitNormal);

#pragma region ObjectPool Interface
	// Render Section
	virtual void ApplyHidden(const bool bHide)override;
	// ObjectPool Setion
	virtual bool IsUseableObject()override;
	virtual void AcquireObject()override;
	virtual void ReleaseObject()override;
#pragma endregion

private:
	// 라이프 사이클 필요없다. 어차피 애니메이션 길이가 바뀔 필요가 없어서 
	// 애니메이션 길이로 라이프 사이클을 잡으면 된다. 
	//FTimerHandle m_hLifeCycle;

	UPROPERTY(meta = (BindWidget))
	UOverlay* Overlay_Text;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Text_Normal;
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Text_Double;


	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_Normal;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_Critical;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_Double;





};
