// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/GC_AttackHit.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UGC_AttackHit::UGC_AttackHit()
{
}

bool UGC_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		if (ParticleSystems.IsEmpty() == true || ParticleScales.IsEmpty() == true)
			return false;

		for(int32 i = 0; i < ParticleSystems.Num(); ++i)
		{
			UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystems[i], HitResult->ImpactPoint, FRotator::ZeroRotator, ParticleScales[i], true);
		}
	}
	else
	{
		if (const FGameplayEffectContext* EfffectContext = Parameters.EffectContext.Get())
		{
			if (EfffectContext->GetActors().IsEmpty() == false)
			{
				for (const auto& TargetActor : EfffectContext->GetActors())
				{
					if (TargetActor.Get())
					{
						if (ParticleSystems.IsEmpty() == true || ParticleScales.IsEmpty() == true)
							return false;

						for (int32 i = 0; i < ParticleSystems.Num(); ++i)
						{
							UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystems[i], TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, ParticleScales[i], true);
						}
					}
				}
			}
			
		}
		
	}

	return false;
}
