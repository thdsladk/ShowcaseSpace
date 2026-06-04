// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_CollisionNotify.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UANS_CollisionNotify : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UANS_CollisionNotify();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

protected:
	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag m_TriggerGameplayTag;

	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag m_EndGameplayTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag m_InflictedStatusTag;

	



	UPROPERTY(EditAnywhere)
	float m_ComboAttackLevel;
};
