// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "UI/DragSlotWidget.h"
#include "Input/Reply.h"

#include "MyTest_TopDownCharacter.h"
#include "MyInventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Character/MyTest_TopDownPlayerController.h"




void USlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

}

void USlotWidget::Init()
{
    AMyTest_TopDownCharacter* MyCharacter = Cast<AMyTest_TopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (MyCharacter != nullptr && MyCharacter->GetInventory() != nullptr)
    {
        m_InventoryComp = MyCharacter->GetInventory();
    }

}

void USlotWidget::UpdateWidget()
{
    // 100�� �ε��� �̸�, ����·� �����.
    if (m_SlotIndex == EmptyIndex)
    {
        SetEmpty();
        return;
    }

    switch (m_SlotType)
    {
        case ESlotType::NoneSlot:
        {
            SetEmpty();
            break;
        }
        case ESlotType::ItemSlot:
        {
            if (m_SlotIndex == EmptyIndex)
            {
                SetEmpty();
                break;
            }
            // ����� ���� 
            SetThumbnail(m_InventoryComp->GetThumbnail(m_SlotIndex));
            // ������ �̸� ����
            SetName(FText::FromString(m_InventoryComp->GetItemName(m_SlotIndex)));
            // ������ ���� ����   
            SetQuantity(FText::FromString(FString::FromInt(m_InventoryComp->GetItemCount(m_SlotIndex))));

            break;
        }
        case ESlotType::EquipmentSlot:
        {

            if (m_SlotIndex == EmptyIndex)
            {
                SetEmpty();
                break;
            }

            // ����� ���� 
            SetThumbnail(m_InventoryComp->GetThumbnail(m_SlotIndex));
            // ������ �̸� ����
            SetName(FText::FromString(m_InventoryComp->GetItemName(m_SlotIndex)));
            // ������ ���� ����   
            SetQuantity(FText::FromString(FString::FromInt(m_InventoryComp->GetItemCount(m_SlotIndex))));

            // �ӽ� �� //


            break;
        }
        case ESlotType::QuickSlot:
        {

            break;
        }


    }



}

FReply USlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FEventReply Reply;
    Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
    {

        if (m_SlotType == ESlotType::EquipmentSlot)
        {
            // ��� ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // ������ ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }

        // Drag On
        Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Drag : Left Button Down"));
    }
    else if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
    {
        if (m_SlotType == ESlotType::EquipmentSlot)
        {
            // ��� ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // ������ ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }

        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Drag : Right Button Down"));
    }

    return Reply.NativeReply;
}

FReply USlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FEventReply Reply;
    Reply.NativeReply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

    if (InMouseEvent.GetEffectingButton() == (EKeys::LeftMouseButton))
    {
        
        if (m_SlotType == ESlotType::EquipmentSlot)
        {
            // ��� ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // ������ ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }
        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Drag : Left Button Up"));
    }
    else if (InMouseEvent.GetEffectingButton() ==(EKeys::RightMouseButton))
    {
        if (m_SlotType == ESlotType::EquipmentSlot)
        {
           // �ڵ����� ��� ���� ������ �������� �ٽ� ������ �ϴ� �ý��� �ʿ�
           // m_InventoryComp->SetEquipmentRef(m_SlotIndex, EmptyIndex);
           // UpdateWidget();
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // ������ ������ �κ��丮���� ����� ����.
            AMyTest_TopDownCharacter* MyCharacter = Cast<AMyTest_TopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
            if (MyCharacter != nullptr)
            {
                MyCharacter->DropItem(m_SlotIndex, {MyCharacter->GetTransform().GetLocation() + FVector(0.0, 0.0, 10.0)});
                UpdateWidget();
            }
        }

        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Drag : Right Button Up"));
    }

    return Reply.NativeReply;
}

FReply USlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FEventReply Reply;
    Reply.NativeReply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
    {

        if (m_SlotType == ESlotType::EquipmentSlot)
        {
            // ��� ���� ��Ŭ���� ���� �ƹ��͵� ����.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // ������ ������ ����Ŭ���ϸ� ĳ���Ͱ� ItemSlot�� ��� �ϵ��� .
            AMyTest_TopDownCharacter* MyCharacter = Cast<AMyTest_TopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
            if (MyCharacter != nullptr)
            {
                MyCharacter->UseItem(m_SlotIndex);
            }
        }
        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, TEXT("Drag : Left Double Click"));
    }

    return Reply.NativeReply;
}

void USlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    if (OutOperation == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging Start"));

        UDragSlotWidget* DragSlot = NewObject<UDragSlotWidget>();
        OutOperation = DragSlot;

        // �巡���� ������ ���� ����
        DragSlot->SetId(m_SlotIndex);
        DragSlot->SetType(static_cast<int32>(m_SlotType));           // Input SlotType.

        // �巡�� ���� ���� 
        USlotWidget* DummySlot = CreateWidget<USlotWidget>(GetWorld()->GetFirstPlayerController(), m_EquipSlotClass);
        if (nullptr != DummySlot)
        {
            DummySlot->m_SlotIndex = m_SlotIndex;
            DummySlot->m_InventoryComp = m_InventoryComp;
            DummySlot->UpdateWidget();
            DragSlot->DefaultDragVisual = DummySlot;
        }
        
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Drag : Draging Again!!!"));
    }

}

bool USlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    bool bResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    int32 DragSlotIndex = CastChecked<UDragSlotWidget>(InOperation)->GetId();
    ESlotType DragSlotType = static_cast<ESlotType>(CastChecked<UDragSlotWidget>(InOperation)->GetType());
    if (m_SlotType == ESlotType::ItemSlot)
    {
        m_InventoryComp->SwapInventory(DragSlotIndex, m_SlotIndex);  
    }
    else if (m_SlotType == ESlotType::EquipmentSlot)
    {
        // �巡�׵� �������� ��񿩾��Ѵ�.  ���� ����ϰ� �Űܹ�����. ���� ������� �����ϴ� �κ��� ����.
        //if ()
        {
            m_InventoryComp->SetEquipmentRef(m_SlotIndex - (m_InventoryComp->GetInventorySize()), DragSlotIndex);
            m_InventoryComp->SwapInventory(DragSlotIndex, m_SlotIndex);            
        }
    }
    
    // Debug Message
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Drop : Dropping"));

    return bResult;
}

void USlotWidget::SetThumbnail(UTexture2D* Texture)
{
    if (Texture == nullptr)
    {
        //Border_Image->SetBrushFromTexture(Texture);
        FSlateBrush Brush;
        Brush.SetResourceObject(Texture);
        Brush.TintColor = FLinearColor(0.1f, 0.1f, 0.1f);
        Border_Image->SetBrush(Brush);
        
    }
    else
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(Texture);
        Brush.TintColor = FLinearColor(1.f, 1.f, 1.f);
        Border_Image->SetBrush(Brush);
    }


}

void USlotWidget::SetName(FText TextBlock)
{
    if (TextBlock.IsEmpty() == false)
        Text_Name->SetText(TextBlock);
}

void USlotWidget::SetQuantity(FText TextBlock)
{
    if(TextBlock.IsEmpty() ==false)
        Text_Quantity->SetText(TextBlock);
}

void USlotWidget::SetEmpty()
{
    // �� �κ��丮 �����ϱ�.
    SetThumbnail(nullptr);
    SetName(FText::FromString(TEXT("Empty")));
    SetQuantity(FText::FromString(TEXT("0")));
}
