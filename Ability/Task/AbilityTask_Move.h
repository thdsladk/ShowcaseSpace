// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAbilityTask.h"
#include "AbilityTask_Move.generated.h"

/**
*    
 * 
 */

UENUM(BlueprintType)
enum class EMoveType : uint8
{
    None			UMETA(DisplayName = "None"),
    Walk			UMETA(DisplayName = "Walk"),
    Sprint			UMETA(DisplayName = "Sprint"),
    CrouchWalk		UMETA(DisplayName = "CrouchWalk"),
    Jump			UMETA(DisplayName = "Jump"),
    Fly				UMETA(DisplayName = "Fly"),
    Swim			UMETA(DisplayName = "Swim"),
    End				UMETA(DisplayName = "End")
};

UCLASS(Blueprintable)
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
    EMoveType m_MoveType = EMoveType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    FVector m_TargetLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    float m_Duration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float m_ElapsedTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    double m_MaxHeight = 900.f; // 포물선 최고 높이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    double m_Distance = 900.f;  // 이동 거리 

    FVector m_StartLocation;

    //EMovementMode m_OriginalMode;


};
