// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_GASAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAnimNotify_GASAttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_GASAttackHitCheck();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

protected:
	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag m_TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag m_InflictedStatusTag;


	UPROPERTY(EditAnywhere)
	float m_ComboAttackLevel;
};
