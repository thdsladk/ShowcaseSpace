// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "JWGASPlayerState.generated.h"

class UAbilitySystemComponent;
class UCharacterAttributeSet;
class UCharacterSkillAttributeSet;


/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AJWGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AJWGASPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	// 블루프린트로 만든다면 어빌리티 컴포넌트를 강제로 초기화 시킬수 있다. 
	/** Ability System */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GAS")
	TObjectPtr <UAbilitySystemComponent> m_AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> m_AttributeSet;
	UPROPERTY()
	TObjectPtr<UCharacterSkillAttributeSet> m_SkillAttributeSet;
};
