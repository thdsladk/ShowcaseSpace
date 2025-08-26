// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

class UMyInventoryComponent;
class UTexture2D;
class FText;
class UBorder;
class UTextBlock;

/**
 * 
 */

UENUM()
enum class ESlotType : int32
{
	NoneSlot,
	ItemSlot,
	EquipmentSlot,
	SkillSlot,
	QuickSlot,
	QItemSlot,
	QSkillSlot
};


UCLASS()
class MYTEST_TOPDOWN_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	void BindInventory(UMyInventoryComponent* InventoryComp);

	UFUNCTION()
	void UpdateWidget();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

	UFUNCTION()
	void SetThumbnail(UTexture2D* Texture);
	void SetName(FText TextBlock);
	void SetQuantity(FText TextBlock);

	void SetEmpty();

	void SetSlotIndex(int32 Index) { m_SlotIndex = Index; }
	void SetSlotType(ESlotType Type) { m_SlotType = Type; }

protected:
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_Image;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Quantity;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_SlotIndex =0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	ESlotType m_SlotType  = ESlotType::NoneSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlotWidget> m_EquipSlotClass;

	//Reference
	UPROPERTY()
	TWeakObjectPtr<UMyInventoryComponent> m_InventoryComp;
	
	
	//UPROPERTY(EditAnywhere)				// Default Slot Image From Editor
	//TWeakObjectPtr<UTexture2D> m_DefaultSlotImage;


};
