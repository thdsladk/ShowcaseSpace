// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "GaugeBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGaugeBarWidget : public UGASUserWidget
{
	GENERATED_BODY()
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnGaugeChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxGaugehanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateGaugeBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> GaugeBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GaugeText;

	float m_CurrentGauge = 0.f;
	float m_CurrentMaxGauge = 0.f;
};
