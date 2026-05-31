// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASBase.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GASBase::UAnimNotify_GASBase()
	:m_Number(1.f)
	,m_NotifyName(TEXT("GASBaseNotify"))
{
}

FString UAnimNotify_GASBase::GetNotifyName_Implementation() const
{
	return m_NotifyName;
}

void UAnimNotify_GASBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor != nullptr)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = m_Number;
			PayloadData.Instigator = OwnerActor;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, m_TriggerGameplayTag, PayloadData);
		}
	}
}
