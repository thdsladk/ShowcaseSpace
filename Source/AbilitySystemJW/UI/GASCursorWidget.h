// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GASCursorWidget.generated.h"

class UImage;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASCursorWidget : public UGASUserWidget
{
	GENERATED_BODY()
public:
	UGASCursorWidget();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

public:
	void InitWidget();
	void UpdateCombatMode();
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Cursor",Meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	FVector2D m_CurrentCursorPos;
};
