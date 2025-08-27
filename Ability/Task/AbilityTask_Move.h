// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/MyAbilityTask.h"
#include "AbilityTask_Move.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UAbilityTask_Move : public UMyAbilityTask
{
	GENERATED_BODY()
public:
    virtual void Start(UMyAbilityInstance* Owner)override;
    virtual void Tick(float DeltaTime)override;
    virtual void Cancel()override;
    virtual bool WantsTick() const override;


protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    FVector m_TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    float m_Duration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float m_ElapsedTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    double m_MaxHeight = 200.f; // 포물선 최고 높이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    double m_Distance = 600.f;  // 이동 거리 

    FVector m_StartLocation;

    EMovementMode m_OriginalMode;
};
