// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_GrantTag.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAnimNotifyState_GrantTag : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_GrantTag();
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

protected:
	UPROPERTY(EditAnywhere, Category = "AnimNotify|Name")
	FString m_NotifyName;
	
	UPROPERTY(EditAnywhere, Category = "AnimNotify|Tag")
	FGameplayTag m_GrantTag;

};
