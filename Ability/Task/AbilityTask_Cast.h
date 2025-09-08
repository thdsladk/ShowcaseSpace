// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/MyAbilityTask.h"
#include "AbilityTask_Cast.generated.h"

class AProjectileBase;
/**
 * 발사체 생성 및 발사 
 */
UCLASS(Blueprintable)
class MYTEST_TOPDOWN_API UAbilityTask_Cast : public UMyAbilityTask
{
	GENERATED_BODY()
public:
    virtual void Start(UMyAbilityInstance* Owner)override;
    virtual void Tick(float DeltaTime)override;
    virtual void Cancel()override;
    virtual bool WantsTick() const override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cast")
    FVector m_Direction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cast")
    float m_Duration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cast")
    float m_ElapsedTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cast")
	TSubclassOf<AProjectileBase> m_ProjectileClass;

    

};
