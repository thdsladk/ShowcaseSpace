// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfManaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfStaminaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfExperienceDelegate);


class UGE_AttackDamage;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxExperience);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Experience);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ChargeGauge);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxChargeGauge);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutOfHealthDelegate OnOutOfHealth;
	mutable FOutOfManaDelegate OnOutOfMana;
	mutable FOutOfStaminaDelegate OnOutOfStamina;
	mutable FOutOfExperienceDelegate OnOutOfExperience;



protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Mana", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "Mana", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, Category = "Experience", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxExperience;

	UPROPERTY(BlueprintReadOnly, Category = "Experience", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Experience;

	UPROPERTY(BlueprintReadOnly, Category = "ChargeGauge", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ChargeGauge;

	UPROPERTY(BlueprintReadOnly, Category = "ChargeGauge", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxChargeGauge;

	// Meta Attribute
	UPROPERTY(BlueprintReadOnly, Category = "Damage", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;




	bool bOutOfHealth = false;
	bool bOutOfMana = false;
	bool bOutOfStamina = false;
	bool bOutOfExperience = false;

	// 이건 GameplayEffect를 C++로 만들 때 접근을 위해서 필요 했던 부분.
	//friend UGE_AttackDamage;
};
