// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TargetActor/Skill/TA_Skill_Radius.h"
#include "AbilitySystemJW.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/JWCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute\CharacterSkillAttributeSet.h"
#include "Engine/OverlapResult.h"
#include "Interface/CollisionTeamInterface.h"
#include "FunctionLibrary/JWFunctionLibrary.h"

ATA_Skill_Radius::ATA_Skill_Radius()
{
}

FGameplayAbilityTargetDataHandle ATA_Skill_Radius::MakeTargetData() const
{
	TArray<FOverlapResult> Overlaps;
	bool HitDetected = false;

	const FTransform& StartTransform = StartLocation.LiteralTransform;
	HitDetected = UJWFunctionLibrary::CheckCollisionTrace_OverlapMultiByChannel(SourceActor, StartTransform.GetLocation(), SkillTargetData.CollisionShape.CollisionShape, m_CollisionChannel, Overlaps, FName("ATA_SphereMultiTrace"));
	TArray<TWeakObjectPtr<AActor>> HitActors;
	FGameplayAbilityTargetData_ActorArray* ActorsData = nullptr;

	if (HitDetected == true)
	{
		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();	//OverlapObjectHandle.FetchActor<AActor>();
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
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		UJWFunctionLibrary::Debug_ShapeSingleByChannel(SourceActor, StartTransform.GetLocation(), StartTransform.GetLocation(), SkillTargetData.CollisionShape.CollisionShape, HitDetected);
	}

#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
