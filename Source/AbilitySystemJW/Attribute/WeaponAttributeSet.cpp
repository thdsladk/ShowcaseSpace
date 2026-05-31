// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/WeaponAttributeSet.h"
#include "WeaponAttributeSet.h"

UWeaponAttributeSet::UWeaponAttributeSet()
	: AttackRange(0.0f)
	, MaxAttackRange(0.0f)
	, AttackRadius(0.0f)
	, MaxAttackRadius(0.0f)
	, AttackRate(0.0f)
	, MaxAttackRate(0.0f)
	, ChargeGauge(0.0f)
	, MaxChargeGauge(1.0f)
	, Damage(0.0f)
{

}
void UWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

	// 데미지의 경우 음수는 막도록 
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}

	if (Attribute == GetAttackRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxAttackRange());
	}
	else if (Attribute == GetAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAttackRate());
	}
	else if(Attribute == GetChargeGaugeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxChargeGauge());
	}

}

bool UWeaponAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if(Super::PreGameplayEffectExecute(Data) == false)
	{
		return false;
	}


	return true;
}

void UWeaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


}


