// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInventoryWidget.h"
#include "ItemSlotWidget.h"
#include "SlotWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"

#include "MyTest_TopDownCharacter.h"
#include "MyInventoryComponent.h"


void UMyInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    m_ItemSlotList.Init(nullptr, m_Row * m_Column);
    m_EquipSlotList.Init(nullptr, 6u);

    FString WidgetNamePattern = TEXT("ItemSlot_"); // �̹��� ���� �̸��� ����
    for (uint32 Index = 0U; Index < (m_Row * m_Column); Index++)
    {
        uint32 Index_Oct = 0U;
        Index_Oct += (Index / 8U)* 10U;
        Index_Oct += (Index % 8U);

        // ItemSlot ������ �̸��� ����
        const FString ItemSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index_Oct);

        // ItemSlot ������ ���ε�
        m_ItemSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*ItemSlotWidgetName));

        // ItemSlot ���� �ʱ�ȭ
        m_ItemSlotList[Index]->Init();

        // ItemSlot Setting
        m_ItemSlotList[Index]->SetSlotIndex(Index);
        
        // ����
        m_ItemSlotList[Index]->UpdateWidget();
    }

    WidgetNamePattern = TEXT("EquipmentSlot_"); // �̹��� ���� �̸��� ����
    for (uint32 Index = 0U; Index < 6U; Index++)
    {
        // EquipSlot ������ �̸��� ����
        const FString EquipSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index);

        // EquipSlot ������ ���ε�
        m_EquipSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*EquipSlotWidgetName));

        // EquipSlot ���� �ʱ�ȭ
        m_EquipSlotList[Index]->Init();

        // EquipSlot Setting        
        m_EquipSlotList[Index]->SetSlotIndex(Index + (m_Row *m_Column));
        m_EquipSlotList[Index]->SetSlotType(ESlotType::EquipmentSlot);

        // ����
        m_EquipSlotList[Index]->UpdateWidget();
    }
   


}

void UMyInventoryWidget::UpdateWidget()
{
    for (auto ItemSlot:m_ItemSlotList)
        ItemSlot->UpdateWidget();

    for (auto EquipmentSlot : m_EquipSlotList)
        EquipmentSlot->UpdateWidget();

}

void UMyInventoryWidget::UpdateWidget_Slot(int32 Index)
{
    if (Index < 0)
    {
        // ������ ������ ���� �� �ִ�.
    }
    else if (Index < int32(m_Row * m_Column))
    {
        m_ItemSlotList[Index]->UpdateWidget();
    }
    else
    {
        m_EquipSlotList[Index- int32(m_Row * m_Column)]->UpdateWidget();
    }
}

void UMyInventoryWidget::BindInventoryComp(UMyInventoryComponent* InventoryComp)
{
    // �κ��丮 ������Ʈ�� �޾Ƽ� ���⼭ ���ε�������.
    if (InventoryComp != nullptr)
    {
        // ���ε� InventoryComponent �� InventoryWidget�� UpdateWidget�Լ��� ���ε�.
        InventoryComp->m_UpdateWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget);
        InventoryComp->m_SwapWidget.BindUObject(this, &UMyInventoryWidget::Swap_ItemSlot);
    }
    
}






