 // Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor/TA_SphereMultiTrace.h"
#include "AbilitySystemJW.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/JWCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute\CharacterSkillAttributeSet.h"
#include "Engine/OverlapResult.h"
#include "Interface/CollisionTeamInterface.h"

#include "FunctionLibrary/JWFunctionLibrary.h"


FGameplayAbilityTargetDataHandle ATA_SphereMultiTrace::MakeTargetData() const
{
	TArray<FOverlapResult> Overlaps;
	bool HitDetected = false;

	const FVector Origin = GetActorLocation(); //SourceActor->GetActorLocation();
	ICollisionTeamInterface* CTI = CastChecked<ICollisionTeamInterface>(SourceActor);

	HitDetected = UJWFunctionLibrary::CheckCollisionTrace_OverlapMultiByChannel(SourceActor,Origin, m_CollisionShape.CollisionShape, CTI->GetHostileCollisionTeambyChannel(), Overlaps, FName("ATA_SphereMultiTrace"));
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
			DrawDebugSphere(GetWorld(), Origin, m_Radius, 16, DrawColor, false, 5.0f);
		}

#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
	
}


