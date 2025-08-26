// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUserWidget.h"
#include "EmotionWidget.generated.h"

class UImage;
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
	void UpdateCommand(uint8 Command);
	void UpdateCharacterMode(uint8 Mode);
	void BindFunction(AActor* Owner);

	void StartEmotion();
	void EndEmotion();

	

private:
	FTimerHandle m_hLifeCycle;
	FVector2D m_Size;
	uint8 m_IsPlay :1;
	uint8 m_IsScalingUp : 1;

	UPROPERTY(meta = (BindWidget))
	UImage* Background;
	UPROPERTY(meta = (BindWidget))
	UImage* Emotion;
};
