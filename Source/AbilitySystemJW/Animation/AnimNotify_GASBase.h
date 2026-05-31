// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_GASBase.generated.h"

/**
 *  특정 게임 플레이 태그를 통해서 이벤트를 발생시키는 클래스 ( 블루프린트로 상속받아서 재사용을 가능하도록 설계 ) 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAnimNotify_GASBase : public UAnimNotify
{
	GENERATED_BODY()
public:

	UAnimNotify_GASBase();
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

protected:
	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag m_TriggerGameplayTag;

	UPROPERTY(EditAnywhere, Meta = (Categories = Number))
	float m_Number;

	UPROPERTY(EditAnywhere, Meta = (Categories = NotifyName))
	FString m_NotifyName;

};
