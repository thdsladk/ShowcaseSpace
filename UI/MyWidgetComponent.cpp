// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyWidgetComponent.h"
#include "UI/MyUserWidget.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* MyUserWidget = Cast<UMyUserWidget>(GetWidget());
	if (MyUserWidget)
	{
		MyUserWidget->SetOwningActor(GetOwner());
	}
}