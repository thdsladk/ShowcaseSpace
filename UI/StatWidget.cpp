// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatWidget.h"
#include "MyStatComponent.h"
#include "Components/TextBlock.h"


bool UStatWidget::Initialize()
{
    bool Result = Super::Initialize();



    return Result;
}

void UStatWidget::NativeConstruct()
{
    Super::NativeConstruct();
    FString WidgetNamePattern = TEXT("Text_Stat"); // 이미지 위젯 이름의 패턴
    m_TextBlockList.Init(nullptr, 9U);
    for (uint32 Index = 0U; Index < 9U; Index++)
    {

        // ItemSlot 위젯의 이름을 생성
        const FString WidgetName = WidgetNamePattern + FString::Printf(TEXT("%d"), Index+1);
    
        m_TextBlockList[Index] = Cast<UTextBlock>(GetWidgetFromName(*WidgetName));
        
    }


}

void UStatWidget::UpdateWidget(const FBaseStatusData& BaseStat, const FBaseStatusData& ModifierStat)
{
    uint32 Index = 0U;

    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("Level"), BaseStat.Level, ModifierStat.Level))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("HP"), BaseStat.HP, ModifierStat.HP))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("MP"), BaseStat.MP, ModifierStat.MP))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("SP"), BaseStat.SP, ModifierStat.SP))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("Attack"), BaseStat.Attack, ModifierStat.Attack))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("Defence"), BaseStat.Defence, ModifierStat.Defence))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("Speed"), BaseStat.Speed, ModifierStat.Speed))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("WillPower"), BaseStat.WillPower, ModifierStat.WillPower))
    );
    m_TextBlockList[Index++]->SetText(
        FText::FromString(FString::Printf(TEXT("%s : %d / %d"), TEXT("HPPPP"), BaseStat.HP, ModifierStat.HP))
    );
}

void UStatWidget::BindStat(UMyStatComponent& StatComp)
{
    if (IsValid(&StatComp))
    {
        // 델리게이트 바인딩
        StatComp.OnStatChanged.AddUObject(this, &UStatWidget::UpdateWidget);

        // 초기 세팅 
        UpdateWidget(StatComp.GetBaseStat(), StatComp.GetModifierStat());

    }

}
