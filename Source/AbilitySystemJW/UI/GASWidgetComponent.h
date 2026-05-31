// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GASWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	virtual void InitWidget()override;

};
