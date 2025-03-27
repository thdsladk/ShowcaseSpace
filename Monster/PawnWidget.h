// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "PawnWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UPawnWidget : public UMyUserWidget
{
	GENERATED_BODY()
public:
	void BindHp(class UMyStatComponent* StatComp);

	void UpdateHp();

private:
	TWeakObjectPtr<class UMyStatComponent> CurrentStatComp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;
};
