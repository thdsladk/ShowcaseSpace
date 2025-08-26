// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHelpTip.h"
#include "Components/TextBlock.h"

void UItemHelpTip::BindHelpTip(FString Name, FString HelpTip)
{
	// 객체를 바인딩 하는것은 아니지만 세팅해주면서 바인딩 방식을 사용하게 될수도 있어서.

	ItemName->SetText(FText::FromString(Name));
	ItemHelpText->SetText(FText::FromString(HelpTip));

}