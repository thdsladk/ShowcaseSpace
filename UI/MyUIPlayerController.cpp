// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UIWidgetClass)
	{
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	}

	if (UIWidgetInstance)
	{
		UIWidgetInstance->AddToViewport();

		//FInputModeUIOnly Mode;
		//Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
		//SetInputMode(Mode);
		//bShowMouseCursor = true;
	}

}
