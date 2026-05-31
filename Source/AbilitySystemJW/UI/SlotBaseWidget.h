// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "SlotBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API USlotBaseWidget : public UGASUserWidget
{
	GENERATED_BODY()
public:
	USlotBaseWidget();

	UFUNCTION(BlueprintCallable, Category = "WidgetAnimation")
	void PlayGaugeAnimation();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotStat")
	float m_CooldownTime;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PlayGauge;
};
