// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UWeaponAttributeSet();

	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, MaxAttackRange);		// 0이면 근접무기
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, AttackRange);			// 
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, MaxAttackRate);		
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, MaxAttackRadius);		// 0이면 점 데미지 0보다 크면 범위 데미지
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, MaxChargeGauge);		
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, ChargeGauge);			// 무기 충전량
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Damage);				//  무기 내구성

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;



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

	UPROPERTY(BlueprintReadOnly, Category = "ChargeGauge", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ChargeGauge;

	UPROPERTY(BlueprintReadOnly, Category = "ChargeGauge", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxChargeGauge;

	// Meta Attribute
	UPROPERTY(BlueprintReadOnly, Category = "Damage", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

};
