// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "EmotionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UEmotionWidget : public UMyUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void DestroyWidget();

	UFUNCTION()
	void UpdateWidget();

	void StartEmotion(float Time);
	void EndEmotion();

private:
	FTimerHandle m_hLifeCycle;
	int32 m_LifeCount = 0;
	FVector2D m_Size;

	UPROPERTY(meta = (BindWidget))
	class UImage* Background;
	UPROPERTY(meta = (BindWidget))
	class UImage* Emotion;
};
