// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UDragSlotWidget : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UDragSlotWidget();
	int32 GetId() { return m_SlotID; }
	void SetId(int32 ID) { m_SlotID = ID; }
	int32 GetType() { return m_SlotType; }
	void SetType(int32 Type){ m_SlotType = Type; }
protected:

	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_SlotID;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_SlotType;



};
