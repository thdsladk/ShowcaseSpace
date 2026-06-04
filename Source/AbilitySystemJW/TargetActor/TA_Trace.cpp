// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor/TA_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "Attribute/CharacterSkillAttributeSet.h"
#include "Attribute/WeaponAttributeSet.h"
#include "DrawDebugHelpers.h"

#include "Interface/CollisionTeamInterface.h"

#include "FunctionLibrary/JWFunctionLibrary.h"

ATA_Trace::ATA_Trace():
	m_TargetAttributeType(EAttributeType::CharacterAttribute),
	m_Range(0.f),
	m_Radius(0.f)
{
}

void ATA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	if (SourceActor != nullptr)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
		if (ASC != nullptr)
		{
			switch (m_TargetAttributeType)
			{
				case EAttributeType::CharacterAttribute:
				{
					const UCharacterAttributeSet* AttributeSet = ASC->GetSet<UCharacterAttributeSet>();
					if (AttributeSet != nullptr)
					{
						m_Range = AttributeSet->GetAttackRange();
						m_Radius = AttributeSet->GetAttackRadius();
					}
					break;
				}
				case EAttributeType::CharacterSkillAttribute:
				{
					const UCharacterSkillAttributeSet* AttributeSet = ASC->GetSet<UCharacterSkillAttributeSet>();
					if (AttributeSet != nullptr)
					{
						m_Range = AttributeSet->GetSkillRange();
						m_Radius = AttributeSet->GetSkillRadius();
					}
					break;
				}
				// (임시) 아직  무기 어트리뷰트는 만들지 않아서 보류
				// case EAttributeType::WeaponAttribute:
				// {
				// }
				default:
				{
					break;
				}
			}
		}
	}

}

void ATA_Trace::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();

	MakeCollisionShape();
	FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
	TargetDataReadyDelegate.Broadcast(DataHandle);
}

FGameplayAbilityTargetDataHandle ATA_Trace::MakeTargetData() const
{
	bool HitDetected = false;
	FHitResult OutHitResult;
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * m_Range);

	// 각 캐릭터의 팀에 의해서 충돌 채널을 결정할 수 있도록 하자 
	ICollisionTeamInterface* CTI = CastChecked<ICollisionTeamInterface>(SourceActor);
	HitDetected = UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(SourceActor,TraceStart,TraceEnd, m_CollisionShape.CollisionShape, CTI->GetHostileCollisionTeambyChannel(), OutHitResult, FName("ATA_Trace"));

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected == true)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (UJWFunctionLibrary::IsDebugMode(SourceActor) == true)
	{
		UJWFunctionLibrary::Debug_ShapeSingleByChannel(SourceActor, m_Range, m_CollisionShape.CollisionShape, HitDetected);
	}

#endif

	return DataHandle;
}

void ATA_Trace::MakeCollisionShape()
{
	m_CollisionShape.ShapeExtent = FVector{ m_Radius, m_Radius, m_Radius };
	m_CollisionShape.MakeCollisionShape();
}