// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHelpTip.h"
#include "Components/TextBlock.h"

void UItemHelpTip::BindHelpTip(FString Name, FString HelpTip)
{
	// ��ü�� ���ε� �ϴ°��� �ƴ����� �������ָ鼭 ���ε� ����� ����ϰ� �ɼ��� �־.

	ItemName->SetText(FText::FromString(Name));
	ItemHelpText->SetText(FText::FromString(HelpTip));

}