// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemHelpTip.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UItemHelpTip : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHelpTip(FString Name, FString HelpTip);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemHelpText;


	
};
