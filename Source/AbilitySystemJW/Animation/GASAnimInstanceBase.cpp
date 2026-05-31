// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GASAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "Interface/PlayerControllerInterface.h"
#include "FunctionLibrary/GASFunctionLibrary.h"

UGASAnimInstanceBase::UGASAnimInstanceBase()
{

}

void UGASAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UGASAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (m_Owner != nullptr)
	{
		IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(m_Owner->GetController());
		if (PCI != nullptr)
		{
			m_CombatModeRef = ECombatMode(PCI->GetCombatMode());
		}
	}

}

bool UGASAnimInstanceBase::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UGASFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}