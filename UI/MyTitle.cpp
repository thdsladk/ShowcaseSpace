// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTitle.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "MyTest_TopDownPlayerController.h"

#include "MyTest_TopDownGameMode.h"

void UMyTitle::GameStart()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TopDownMap"));

}



void UMyTitle::GameRetry()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TopDownMap");

}

void UMyTitle::GameEnd()
{
	UWorld* CurWorld = GetWorld();
	UKismetSystemLibrary::QuitGame(CurWorld,CurWorld->GetFirstPlayerController()
		, EQuitPreference::Quit, false);
}

void UMyTitle::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Start->OnClicked.AddDynamic(this, &UMyTitle::GameStart);
	Btn_Retry->OnClicked.AddDynamic(this, &UMyTitle::GameRetry);
	Btn_End->OnClicked.AddDynamic(this, &UMyTitle::GameEnd);

}
