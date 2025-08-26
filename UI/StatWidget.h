// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

struct FBaseStatusData;
class UMyStatComponent;


/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize()override;
	virtual void NativeConstruct() override;


	UFUNCTION()
	void UpdateWidget(const FBaseStatusData& BaseStat , const FBaseStatusData& ModifierStat );

	void BindStat(UMyStatComponent& StatComp);


protected:
	UPROPERTY()
	TArray<class UTextBlock*> m_TextBlockList;
};
