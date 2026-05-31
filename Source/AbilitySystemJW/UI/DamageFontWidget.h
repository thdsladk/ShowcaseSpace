// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "DamageFontWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UDamageFontWidget : public UGASUserWidget
{
	GENERATED_BODY()
public:
	UDamageFontWidget();
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
	void InitializeDamageFont(float InDamage);
	void PlayDamageFont();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void UpdateTextBlock();


	// 정리 로직을 한 곳에 모음
	void CleanupAndRemove();

	UFUNCTION()
	void OnLifeTimeEnded();

	protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <UWidgetAnimation> Upper;
	
	FDelegateHandle m_DamageChangeHandle;

	float m_CurrentDamage = 0.f;
	FLinearColor m_NormalColor = FLinearColor::Red;
	FLinearColor m_CriticalColor = FLinearColor(255.f, 165.f, 0.f, 255.f);
	FLinearColor m_PoisonColor = FLinearColor::Green;
	FLinearColor m_HealColor = FLinearColor(1.0f, 1.0f, 0.4f, 1.0f);
	FLinearColor m_MissColor = FLinearColor::White;


	uint8 m_IsPlay : 1;
	uint8 m_IsMissed : 1;
};
