// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/TargetActor/Skill/TA_Skill_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "Header/GASStructTypes.h"

ATA_Skill_Trace::ATA_Skill_Trace()
{
}

FGameplayAbilityTargetDataHandle ATA_Skill_Trace::MakeTargetData() const
{
	bool HitDetected = false;
	FHitResult OutHitResult;
	
	const FTransform& StartTransform = StartLocation.LiteralTransform;
	FVector End = StartTransform.GetLocation() + (StartTransform.GetRotation().GetForwardVector() * m_Range);
	
	// 각 캐릭터의 팀에 의해서 충돌 채널을 결정할 수 있도록 하자 
	HitDetected = UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(SourceActor, StartTransform.GetLocation(), End, SkillTargetData.CollisionShape.CollisionShape,m_CollisionChannel, OutHitResult, FName("ATA_Skill_Trace"),m_bIgnoreSelf);

	FGameplayAbilityTargetDataHandle DataHandle;

	if (HitDetected == true)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}	

#if ENABLE_DRAW_DEBUG

	if (UJWFunctionLibrary::IsDebugMode(SourceActor) == true)
	{
		UJWFunctionLibrary::Debug_ShapeSingleByChannel(SourceActor, m_Range, SkillTargetData.CollisionShape.CollisionShape, HitDetected);
	}

#endif

	return DataHandle;
}
