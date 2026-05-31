// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_GASStopMove.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAnimNotify_GASStopMove : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_GASStopMove();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

protected:
	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag m_TriggerGameplayTag;

};
