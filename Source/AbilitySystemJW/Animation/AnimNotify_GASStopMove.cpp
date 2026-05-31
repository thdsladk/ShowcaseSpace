// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASStopMove.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GASStopMove::UAnimNotify_GASStopMove()
{
}

FString UAnimNotify_GASStopMove::GetNotifyName_Implementation() const
{
	return TEXT("GASStopMove");
}

void UAnimNotify_GASStopMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor != nullptr)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, m_TriggerGameplayTag, PayloadData);
		}
	}
}
