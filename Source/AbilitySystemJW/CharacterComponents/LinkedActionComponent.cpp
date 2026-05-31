// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/LinkedActionComponent.h"
//#include "AbilitySystemInterface.h"
#include "Character/GASPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemJW.h"
#include "Header/JWHeader.h"

// Sets default values for this component's properties
ULinkedActionComponent::ULinkedActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	m_IsActiveLinkedAction = false;
	// ...
}

void ULinkedActionComponent::AddAction(int32 InputID)
{
	// 무조건 연계액션을 다 쌓고 판별은 나중에 한다. 
	if(m_InputIDs.IsEmpty() == true)
	{
		m_IsActiveLinkedAction = true;	
	}
	m_InputIDs.Emplace(InputID);
}

int32 ULinkedActionComponent::PopAction()
{	
	if (m_InputIDs.IsEmpty() == true)
	{
		return -1;
	}

	// 액션이 다 소모되면 연계액션 종료.
	if (m_InputIDs.Num() == 1)
	{
		m_IsActiveLinkedAction = false;
	}
	return m_InputIDs.Pop(true);
}

void ULinkedActionComponent::EndAction(int32 InputID)
{
	if (m_InputIDs.IsEmpty() == false)
	{
		int32& NewInputID = *(m_InputIDs.begin());
		if (InputID == NewInputID)
		{
			m_InputIDs.RemoveAt(0);
			return;
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULinkedActionComponent::ExcuteAction);
	}
}

void ULinkedActionComponent::ExcuteAction()
{
	if (m_InputIDs.IsEmpty() == false)
	{
		int32& NewInputID = *(m_InputIDs.begin());
		UAbilitySystemComponent* ASC = CastChecked<AGASPlayerCharacter>(GetOwner())->GetAbilitySystemComponent();
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(NewInputID);
		if (Spec != nullptr)
		{
			Spec->InputPressed = true;
			Spec->InputID = NewInputID;

			if (Spec->IsActive() == false)
			{
				m_InputIDs.RemoveAt(0);
				if (ASC->TryActivateAbility(Spec->Handle) == false)
				{
					// 어빌리티 활성화 실패시.
					Debug::Print(Spec->GetDebugString() + TEXT(" Fail"), FColor::Red);
				}
			}
		}

		if (m_InputIDs.Num() <= 0)
		{
			m_InputIDs.Empty();
			m_IsActiveLinkedAction = false;
		}
	}
}

