// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
	: m_ComboAttackLevel(1.0f)
{
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASHit");
}

void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor != nullptr)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = m_ComboAttackLevel;
			PayloadData.Instigator = OwnerActor;
			PayloadData.TargetTags.AddTag(m_InflictedStatusTag);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, m_TriggerGameplayTag, PayloadData);
		}
	}
}
