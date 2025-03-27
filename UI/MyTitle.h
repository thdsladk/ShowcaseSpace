// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTitle.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyTitle : public UUserWidget
{
	GENERATED_BODY()
public:


	UFUNCTION()
		void GameStart();
	UFUNCTION()
		void GameRetry();
	UFUNCTION()
		void GameEnd();
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Start;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Retry;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Retry;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_End;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_End;

};
