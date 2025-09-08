// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAbilityTask.h"
#include "AbilityTask_PlayEffect.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USoundBase;
/**
 * 
 */
UCLASS(Blueprintable)
class MYTEST_TOPDOWN_API UAbilityTask_PlayEffect : public UMyAbilityTask
{
	GENERATED_BODY()
public:
    virtual void Start(UMyAbilityInstance* Owner)override;
    virtual void Tick(float DeltaTime)override;
    virtual void Cancel()override;
    virtual bool WantsTick() const override;


public:
    void SpawnEffect();


protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TWeakObjectPtr <UNiagaraSystem> m_NiagaraTemplate;
    
    TWeakObjectPtr <UNiagaraComponent> m_NiagaraComp;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TWeakObjectPtr <USoundBase> m_Sound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName m_AttachSocketName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FVector m_LocationOffset;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FRotator m_RotationOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    TWeakObjectPtr<AActor> m_Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    uint8 m_bAttachToTarget : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    uint8 m_bAutoDestroy : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    uint8 m_bEffectSpawned : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float m_Duration = 0.f; // 0이면 즉시 종료

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float m_StartDelay = 0.f;


    float m_ElapsedTime = 0.f;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    //UMaterialInterface* m_DecalMaterial;

};
