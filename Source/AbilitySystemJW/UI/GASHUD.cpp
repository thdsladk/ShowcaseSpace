// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASHUD.h"
#include "AbilitySystemComponent.h"
#include "Components/Button.h"
#include "UI/SlotBaseWidget.h"
#include "UI/GASGaugeBarUserWidget.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Tag/JWGameplayTag.h"

UGASHUD::UGASHUD()
{
}

void UGASHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// HUD에 포커스를 주기.
	SetKeyboardFocus();

	if (m_AbilitySystemComponent != nullptr)
	{
		AActor* OwnerActor = m_AbilitySystemComponent->GetAvatarActor();
		if (OwnerActor != nullptr)
		{
			SkillSlotQ->SetAbilitySystemComponent(OwnerActor);
			SkillSlotE->SetAbilitySystemComponent(OwnerActor);
			SkillSlotR->SetAbilitySystemComponent(OwnerActor);
			SkillSlotX->SetAbilitySystemComponent(OwnerActor);

			m_SkillSlots.Add(EKeys::Q, SkillSlotQ);
			m_SkillSlots.Add(EKeys::E, SkillSlotE);
			m_SkillSlots.Add(EKeys::R, SkillSlotR);
			m_SkillSlots.Add(EKeys::X, SkillSlotX);

			GaugeBar->SetAbilitySystemComponent(OwnerActor);

		}

		// 스킬이 발동되면 콜백하도록 세팅
		//m_AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(JWTAG_EVENT_CHARACTER_SKILL_FLOAT).AddUObject(this, &UGASHUD::SkillQ);
		//m_AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(JWTAG_EVENT_CHARACTER_SKILL_STUN).AddUObject(this, &UGASHUD::SkillE);
		//m_AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(JWTAG_EVENT_CHARACTER_SKILL_AROUND).AddUObject(this, &UGASHUD::SkillR);
		//m_AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(JWTAG_EVENT_CHARACTER_SKILL_DASHATTACK).AddUObject(this, &UGASHUD::SkillX);
		m_AbilitySystemComponent->RegisterGameplayTagEvent(JWTAG_CHARACTER_ACTION_SKILLQ, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASHUD::SkillQ);
		m_AbilitySystemComponent->RegisterGameplayTagEvent(JWTAG_CHARACTER_ACTION_SKILLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASHUD::SkillE);
		m_AbilitySystemComponent->RegisterGameplayTagEvent(JWTAG_CHARACTER_ACTION_SKILLR, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASHUD::SkillR);
		m_AbilitySystemComponent->RegisterGameplayTagEvent(JWTAG_CHARACTER_ACTION_SKILLX, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASHUD::SkillX);

			
	}

}
/*
void UGASHUD::SkillQ(const FGameplayEventData* Payload)
{
	UButton* Btn_Skill = Cast<UButton>(SkillSlotQ->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotQ->PlayGaugeAnimation();
		}
	}
}

void UGASHUD::SkillE(const FGameplayEventData* Payload)
{
	UButton* Btn_Skill = Cast<UButton>(SkillSlotE->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotE->PlayGaugeAnimation();
		}
	}
}

void UGASHUD::SkillR(const FGameplayEventData* Payload)
{
	UButton* Btn_Skill = Cast<UButton>(SkillSlotR->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotR->PlayGaugeAnimation();
		}
	}
}

void UGASHUD::SkillX(const FGameplayEventData* Payload)
{
	UButton* Btn_Skill = Cast<UButton>(SkillSlotX->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotX->PlayGaugeAnimation();
		}
	}
}
*/

void UGASHUD::SkillQ(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 1 || NewCount == 0) return;
	UButton* Btn_Skill = Cast<UButton>(SkillSlotQ->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotQ->PlayGaugeAnimation();
		}
	}
}

void UGASHUD::SkillE(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 1 || NewCount == 0) return;
	UButton* Btn_Skill = Cast<UButton>(SkillSlotE->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotE->PlayGaugeAnimation();
		}
	}
}

void UGASHUD::SkillR(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 1 || NewCount == 0) return;
	UButton* Btn_Skill = Cast<UButton>(SkillSlotR->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotR->PlayGaugeAnimation();
		}
	}
}

void UGASHUD::SkillX(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 1 || NewCount == 0) return;
	UButton* Btn_Skill = Cast<UButton>(SkillSlotX->GetWidgetFromName(FName(TEXT("Btn_Skill"))));
	if (Btn_Skill != nullptr)
	{
		// 버튼이 활성화 되어 있을때만 호출하도록.
		if (Btn_Skill->GetVisibility() == ESlateVisibility::Visible)
		{
			Btn_Skill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillSlotX->PlayGaugeAnimation();
		}
	}
}
