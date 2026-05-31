// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterSkillAttributeSet();

	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, SkillRange);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, SkillRadius);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, MaxSkillRadius);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, SkillAttackRate);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, MaxSkillAttackRate);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, SkillEnergy);
	ATTRIBUTE_ACCESSORS(UCharacterSkillAttributeSet, MaxSkillEnergy);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy;

	UPROPERTY(BlueprintReadOnly, Category = "Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;

};
