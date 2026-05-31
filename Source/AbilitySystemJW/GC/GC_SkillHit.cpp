// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/GC_SkillHit.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

UGC_SkillHit::UGC_SkillHit()
{
}

bool UGC_SkillHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		for (int32 i = 0; i < ParticleProperties.Num(); ++i)
		{
			UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleProperties[i].ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, ParticleProperties[i].Scale, true);
		}

		for(int32 i =0; i < NiagaraProperties.Num(); ++i)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraProperties[i].NiagaraSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, NiagaraProperties[i].Scale, true, true, ENCPoolMethod::AutoRelease);
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
						for (int32 i = 0; i < ParticleProperties.Num(); ++i)
						{
							UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleProperties[i].ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, ParticleProperties[i].Scale, true);
						}

						for (int32 i = 0; i < NiagaraProperties.Num(); ++i)
						{
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraProperties[i].NiagaraSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, NiagaraProperties[i].Scale, true, true, ENCPoolMethod::AutoRelease);
						}
					}
				}
			}
		}
	}
	return false;
}
