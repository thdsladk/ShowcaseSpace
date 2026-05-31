// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_SkillHit.generated.h"

class UNiagaraSystem;
class UParticleSystem;


USTRUCT(BlueprintType)
struct FParticleSystemProperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UParticleSystem> ParticleSystem;
	UPROPERTY(EditAnywhere, Category = GameplayCue)
	FVector Scale;
};

USTRUCT(BlueprintType)
struct FNiagaraSystemProperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
	UPROPERTY(EditAnywhere, Category = GameplayCue)
	FVector Scale;
};

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGC_SkillHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UGC_SkillHit();
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TArray<FParticleSystemProperty> ParticleProperties;

	// Niagara 시스템 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TArray<FNiagaraSystemProperty> NiagaraProperties;

};
