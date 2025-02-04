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
	void BindHp(class UMonsterStatComponent* StatComp);

	void UpdateHp();

private:
	TWeakObjectPtr<class UMonsterStatComponent> CurrentStatComp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;
};
