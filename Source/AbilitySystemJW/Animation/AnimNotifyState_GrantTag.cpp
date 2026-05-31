// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_GrantTag.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


UAnimNotifyState_GrantTag::UAnimNotifyState_GrantTag()
	:m_NotifyName("GrantTag")
{
}

FString UAnimNotifyState_GrantTag::GetNotifyName_Implementation() const
{
	return m_NotifyName;
}

void UAnimNotifyState_GrantTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 여기서 Tag 부여
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (ASC != nullptr)
	{
		ASC->AddLooseGameplayTag(m_GrantTag);
	}
}

void UAnimNotifyState_GrantTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 여기서 Tag 제거
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (ASC != nullptr)
	{
		ASC->RemoveLooseGameplayTag(m_GrantTag);
	}
}
