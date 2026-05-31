// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GASHUD.generated.h"

class USlotBaseWidget;
class UGASGaugeBarUserWidget;
struct FGameplayEventData;

/**
 *     이것도 하드코딩의 문제가 있다 . 
 *		특정 위젯과 버튼을 부모 클래스에서 특정해서 세팅을 해주면 자식 블루프린트를 자유롭게 유동적으로 구성할때 제한이 될 수 있다. 
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASHUD : public UGASUserWidget
{
	GENERATED_BODY()
public:
	UGASHUD();
	virtual void NativeConstruct() override;

	void SkillQ(const FGameplayEventData* Payload);
	void SkillE(const FGameplayEventData* Payload);
	void SkillR(const FGameplayEventData* Payload);
	void SkillX(const FGameplayEventData* Payload);
protected:
	UPROPERTY(meta = (BindWidget))
	USlotBaseWidget* SkillSlotQ;

	UPROPERTY(meta = (BindWidget))
	USlotBaseWidget* SkillSlotE;
	
	UPROPERTY(meta = (BindWidget))
	USlotBaseWidget* SkillSlotR;
	
	UPROPERTY(meta = (BindWidget))
	USlotBaseWidget* SkillSlotX;

	UPROPERTY(meta = (BindWidget))
	UGASGaugeBarUserWidget* GaugeBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotList")
	TMap<FKey, USlotBaseWidget*> m_SkillSlots;


};
