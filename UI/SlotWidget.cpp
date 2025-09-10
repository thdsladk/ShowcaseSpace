// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "UI/DragSlotWidget.h"
#include "Input/Reply.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"

#include "Character/MyTest_TopDownPlayerController.h"
#include "UI/MyInventoryWidget.h"
#include "Interface/InventoryInterface.h"
#include "Interface/BehaviorInterface.h"
#include "CharacterComponent/MyInventoryComponent.h"

// define 
#define EmptyIndex 100

bool USlotWidget::Initialize()
{
    bool Result = Super::Initialize();


    return Result;
}

void USlotWidget::NativeConstruct()
{
    Super::NativeConstruct();


}

void USlotWidget::BindInventory(UMyInventoryComponent* InventoryComp)
{
    m_InventoryComp = InventoryComp;
}

void USlotWidget::UpdateWidget()
{
    // 100번 인덱스 이면, 빈상태로 만들기.
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
            // 썸네일 세팅 
            SetThumbnail(m_InventoryComp->GetThumbnail(m_SlotIndex));
            // 아이템 이름 세팅
            SetName(FText::FromString(m_InventoryComp->GetItemName(m_SlotIndex)));
            // 아이템 갯수 세팅   
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

            // 썸네일 세팅 
            SetThumbnail(m_InventoryComp->GetThumbnail(m_SlotIndex));
            // 아이템 이름 세팅
            SetName(FText::FromString(m_InventoryComp->GetItemName(m_SlotIndex)));
            // 아이템 갯수 세팅   
            SetQuantity(FText::FromString(FString::FromInt(m_InventoryComp->GetItemCount(m_SlotIndex))));

            // 임시 끝 //


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
            // 장비 슬롯 좌클릭은 아직 아무것도 없다.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // 아이템 슬롯 우클릭은 아직 아무것도 없다.
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
            // 장비 슬롯 우클릭은 아직 아무것도 없다.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // 아이템 슬롯 우클릭은 아직 아무것도 없다.
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
            // 장비 슬롯 좌클릭은 아직 아무것도 없다.
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // 아이템 슬롯 좌클릭은 아직 아무것도 없다.
        }
        else if( m_SlotType == ESlotType::QuickSlot)
        {
            // 퀵슬롯 좌클릭은 아직 아무것도 없다.
		}
        else if (m_SlotType == ESlotType::SkillSlot)
        {
            // 스킬슬롯 좌클릭은 아직 아무것도 없다.
            //APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
            //if(Pawn != nullptr)
            //{
            //    IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(Pawn);
            //    BI->UseSkill(m_SlotIndex);
			//}
        }
        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Drag : Left Button Up"));
    }
    else if (InMouseEvent.GetEffectingButton() ==(EKeys::RightMouseButton))
    {
        if (m_SlotType == ESlotType::EquipmentSlot)
        {
           // 자동으로 장비를 벗고 아이템 슬롯으로 다시 들어가도록 한다.
           //m_InventoryComp->UnEquip(m_SlotIndex);
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            //// 아이템 슬롯을 인벤토리에서 벗어나게 하자.
            //APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
            //if (Pawn != nullptr)
            //{
            //    CastChecked<IInventoryInterface>(Pawn)->DropItem(m_SlotIndex, {Pawn->GetTransform().GetLocation() + FVector(0.0, 0.0, 10.0)});
            //    
            //}
        }
        else if (m_SlotType == ESlotType::QuickSlot)
        {
            // 퀵슬롯 우클릭은 아직 아무것도 없다.
        }
        else if (m_SlotType == ESlotType::SkillSlot)
        {
            // 스킬슬롯 우클릭은 아직 아무것도 없다.
        }

        UpdateWidget();
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
            // 장비 슬롯 좌클릭은 아직 아무것도 없다.

        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // 슬롯의 왼쪽을 더블클릭하면 캐릭터가 ItemSlot을 사용 하도록 .
            APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
            if (Pawn != nullptr)
            {
                IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(Pawn);
                EItemType ItemType = static_cast<EItemType>(InventoryInterface->GetItemType(m_SlotIndex));
                if (ItemType == EItemType::ConsumableItem)
                {
                    InventoryInterface->UseItem(m_SlotIndex);
                }
                else if (ItemType == EItemType::EquipmentItem)
                {
                    m_InventoryComp->Equip(m_SlotIndex);
                }
            }
        }

        UpdateWidget();
        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, TEXT("Drag : Left Double Click"));
    }
    else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
    {
        if (m_SlotType == ESlotType::EquipmentSlot)
        {
            // 장비 슬롯 오른쪽 더블클릭은 장비 해제.
            m_InventoryComp->UnEquip(m_SlotIndex);
        }
        else if (m_SlotType == ESlotType::ItemSlot)
        {
            // 아이템 슬롯 오른쪽 더블클릭은 아이템 버리기.
            APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
            if (Pawn != nullptr)
            {
                CastChecked<IInventoryInterface>(Pawn)->DropItem(m_SlotIndex, { Pawn->GetTransform().GetLocation() + FVector(0.0, 0.0, 10.0) });
            }
        }

        //Debug
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, TEXT("Drag : Right Double Click"));
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

        // 드래그한 아이템 정보 전달
        DragSlot->SetId(m_SlotIndex);
        DragSlot->SetType(static_cast<int32>(m_SlotType));           // Input SlotType.

        // 드래그 슬롯 복사 
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
        if (DragSlotType == ESlotType::ItemSlot)
        {
            m_InventoryComp->SwapInventory(DragSlotIndex, m_SlotIndex); 
        }
        else if (DragSlotType == ESlotType::EquipmentSlot)
        {
            m_InventoryComp->UnEquip(DragSlotIndex, m_SlotIndex);
        }
    }
    else if (m_SlotType == ESlotType::EquipmentSlot)
    {
        // 드래그된 아이템이 장비여야한다.  장비로 등록하고 옮겨버리자. 아직 장비인지 구분하는 부분이 없다.
            if (m_InventoryComp->Equip(DragSlotIndex, m_SlotIndex) == true)
        {
            // 장착 성공
            //Debug
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Equip : Success "));
        }
        else
        {
            // 장착 실패
            //Debug
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Equip : Fail "));
        }
    }

    UpdateWidget();

    // Debug Message
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Drop : Dropping"));

    return bResult;
}

void USlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

    // 태그로 슬롯이 위젯의 영역을 벗어났는지 구분하도록 하고 있다.
    if (InOperation->Tag.Equals(FString{ TEXT("LeaveWidget") }) == true)
    {
        // 아이템 슬롯을 인벤토리에서 벗어나게 하자.
        APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        if (Pawn != nullptr)
        {
                CastChecked<IInventoryInterface>(Pawn)->DropItem(m_SlotIndex, { Pawn->GetTransform().GetLocation() + FVector(0.0, 0.0, 10.0) });
        }
        UpdateWidget();
    }

    // Debug Message
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Drop : Drop Item"));

}

void USlotWidget::SetThumbnail(UTexture2D* Texture)
{
    if (Texture == nullptr)
    {
        //Border_Image->SetBrushFromTexture(Texture);
        FSlateBrush Brush;
        Brush.SetResourceObject(Texture);
        Brush.TintColor = FLinearColor(0.1f, 0.1f, 0.1f,0.0f);
        Border_Image->SetBrush(Brush);
        
    }
    else
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(Texture);
        Brush.TintColor = FLinearColor(1.f, 1.f, 1.f,1.f);
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
    // 빈 인벤토리 세팅하기.
    SetThumbnail(nullptr);
    SetName(FText::FromString(TEXT("Empty")));
    SetQuantity(FText::FromString(TEXT("0")));
}
