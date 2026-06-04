// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_Base.generated.h"

class AAOEBase;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_Base : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ATA_Base();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
public:
	TEnumAsByte<EGameplayTargetingConfirmation::Type> GetConfirmationType() const { return m_ConfirmationType; }
	void SetConfirmationType(TEnumAsByte<EGameplayTargetingConfirmation::Type> InConfirmationType) { m_ConfirmationType = InConfirmationType; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldRecticle", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	TSubclassOf<AAOEBase> m_WorldRecticleClass;
	TObjectPtr<AAOEBase> m_WorldRecticle;

	TEnumAsByte<EGameplayTargetingConfirmation::Type> m_ConfirmationType;
};
