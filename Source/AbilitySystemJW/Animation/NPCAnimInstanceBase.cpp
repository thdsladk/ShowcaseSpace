// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NPCAnimInstanceBase.h"
#include "FunctionLibrary/GASFunctionLibrary.h"

UNPCAnimInstanceBase::UNPCAnimInstanceBase()
{
}

void UNPCAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UNPCAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

bool UNPCAnimInstanceBase::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UGASFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}
