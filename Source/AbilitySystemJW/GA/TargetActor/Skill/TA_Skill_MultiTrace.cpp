// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TargetActor/Skill/TA_Skill_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "Header/GASStructTypes.h"

ATA_Skill_MultiTrace::ATA_Skill_MultiTrace()
{
}

FGameplayAbilityTargetDataHandle ATA_Skill_MultiTrace::MakeTargetData() const
{
	bool HitDetected = false;
	TArray<FHitResult> OutHitResults;
	FGameplayAbilityTargetData_ActorArray* ActorsData = nullptr;
	TArray<TWeakObjectPtr<AActor>> HitActors;

	const FTransform& StartTransform = StartLocation.LiteralTransform;
	FVector End = StartTransform.GetLocation() + (StartTransform.GetRotation().GetForwardVector() * m_Range);

	// 각 캐릭터의 팀에 의해서 충돌 채널을 결정할 수 있도록 하자 
	HitDetected = UJWFunctionLibrary::CheckCollisionTrace_SweepMultiByChannel(SourceActor, StartTransform.GetLocation(), End, SkillTargetData.CollisionShape.CollisionShape, m_CollisionChannel, OutHitResults, FName("ATA_Skill_MultiTrace"), m_bIgnoreSelf);

	if (HitDetected == true)
	{
		for (const FHitResult& HitResult : OutHitResults)
		{
			AActor* HitActor = HitResult.GetActor();	//OverlapObjectHandle.FetchActor<AActor>();
			if (HitActor != nullptr && HitActors.Contains(HitActor) == false)
			{
				HitActors.Add(HitActor);
			}
		}

		ActorsData = new FGameplayAbilityTargetData_ActorArray();
		ActorsData->SetActors(HitActors);
	}


#if ENABLE_DRAW_DEBUG

	if (UJWFunctionLibrary::IsDebugMode(SourceActor) == true)
	{
		UJWFunctionLibrary::Debug_ShapeSingleByChannel(SourceActor, m_Range, SkillTargetData.CollisionShape.CollisionShape, HitDetected);
	}

#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
