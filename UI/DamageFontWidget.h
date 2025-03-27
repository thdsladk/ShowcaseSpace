// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "DamageFontWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UDamageFontWidget : public UMyUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void DestroyWidget();

	UFUNCTION()
		void UpdateWidget();

	void TakeDamage(int32 Value);

private:
	FTimerHandle m_hLifeCycle;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Normal;


};
