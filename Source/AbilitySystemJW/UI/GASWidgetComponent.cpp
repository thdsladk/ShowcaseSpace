// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASWidgetComponent.h"
#include "UI/GASUserWidget.h"

void UGASWidgetComponent::InitWidget()
{
	Super::InitWidget();
	UGASUserWidget* GASWidget = Cast<UGASUserWidget>(GetWidget());
	if (GASWidget != nullptr)
	{
		GASWidget->SetAbilitySystemComponent(GetOwner());
	}


}
