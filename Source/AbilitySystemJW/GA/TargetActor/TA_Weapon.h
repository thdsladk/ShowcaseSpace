// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TargetActor/TA_Base.h"
#include "TA_Weapon.generated.h"

class AGASWeaponBase;

DECLARE_DELEGATE_OneParam(FOnOverlapBegin,AActor*);
DECLARE_DELEGATE_OneParam(FOnOverlapEnd, AActor*);

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_Weapon : public ATA_Base
{
	GENERATED_BODY()
public:
	ATA_Weapon();
	void SetTargetActor(AActor* InTargetActor) { m_OverlappedActors.Add(InTargetActor); }

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	//void SetShowDebug(bool InShowDebug) { m_bShowDebug = InShowDebug; }

		// 오버랩 이벤트 핸들러
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const override;

public:
	FOnOverlapBegin OnOverlapBegin;
	FOnOverlapEnd OnOverlapEnd;

protected:
	TArray<TWeakObjectPtr<AActor>> m_OverlappedActors;
	TArray<TWeakObjectPtr<UShapeComponent>> m_OverlappedComponents;
};
