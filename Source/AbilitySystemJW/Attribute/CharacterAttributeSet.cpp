// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/CharacterAttributeSet.h"
#include "AbilitySystemJW.h"
#include "GameplayEffectExtension.h"
#include "Character/CharacterBase.h"
#include "CharacterComponents/PhysicsMotionSystemComponent.h"
#include "Tag/JWGameplayTag.h"


UCharacterAttributeSet::UCharacterAttributeSet()
	: AttackRange(100.0f)
	, MaxAttackRange(300.0f)
	, AttackRadius(50.f)
	, MaxAttackRadius(150.0f)
	, AttackRate(30.0f)
	, MaxAttackRate(100.0f)
	, MaxHealth(100.0f)
	, MaxMana(100.0f)
	, MaxStamina(100.0f)
	, ChargeGauge(0.0f)
	, MaxChargeGauge(1.0f)
	, Damage(0.0f)
{
	InitHealth(GetMaxHealth());
	InitMana(GetMaxMana());
	InitStamina(GetMaxStamina());
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 체력일 경우 최소 값과 최대 값 사이로 고정시키기.
	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	//}

	// 데미지의 경우 음수는 막도록 
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Super::PreGameplayEffectExecute(Data)==false)
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			ACharacterBase* OwnerActor = CastChecked<ACharacterBase>(GetOwningAbilitySystemComponentChecked()->GetAvatarActor());

			// (임시) 과연 오버헤드 위치가 문제가 없을지... 
			OwnerActor->PlayDamageFont(Data.EvaluatedData.Magnitude, OwnerActor->GetMesh()->GetSocketLocation(TEXT("OverHead")));
		}
	}

	return true;
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const float MinimumGauge = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		JWGAS_LOG(LogJWGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumGauge, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		JWGAS_LOG(LogJWGAS, Warning, TEXT("Direct Mana Access : %f"), GetMana());
		SetMana(FMath::Clamp(GetMana(), MinimumGauge, GetMaxMana()));

	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		JWGAS_LOG(LogJWGAS, Warning, TEXT("Direct Stamina Access : %f"), GetStamina());
		SetStamina(FMath::Clamp(GetStamina(), MinimumGauge, GetMaxStamina()));

	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// 이시점에 데미지가 들어온다는걸 알 수 있다. 
		JWGAS_LOG(LogJWGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumGauge, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(JWTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}
