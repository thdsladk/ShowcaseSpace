// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "PawnWidget.generated.h"

class UMyStatComponent;
class UProgressBar;

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UPawnWidget : public UMyUserWidget
{
	GENERATED_BODY()
public:
	void BindHp(UMyStatComponent* StatComp);

	void UpdateHp(float Hp_Ratio);
	void UpdateAggroGauge(uint8 Gauge);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_HpBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_AggroBar;

};
