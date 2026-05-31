// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "GASDumy.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AGASDumy : public ACharacterBase 
{
	GENERATED_BODY()
protected:
	AGASDumy(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	virtual void TimerAction();



protected:

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class URotatingMovementComponent> m_RotatingMovement;

	UPROPERTY(EditAnywhere, Category = Timer)
	double m_ActionPeriod;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> m_StartAbilities;

	FTimerHandle m_ActionTimer;
};
