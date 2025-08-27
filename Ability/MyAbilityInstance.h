// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityComponent.h"
#include "MyAbilityInstance.generated.h"

class UMyAbilityTask;

/**
 * 
 */

UENUM()
enum class EAbilityState : uint8 
{ 
    Spawning, 
    Activating, 
    Running, 
    Ending, 
    Canceled, 
    Finished 
};

UCLASS()
class MYTEST_TOPDOWN_API UMyAbilityInstance : public UObject
{
	GENERATED_BODY()
public:
    void Initialize(UAbilityComponent* pAbilityComp,UMyAbilityData* pData, FAbilityHandle Handle);
    EAbilityActivateResult Activate();
    void Tick(float DeltaTime);
    void End(bool bCanceled /*, FGameplayTag Reason*/);
    bool IsCancelable() const;

    // Events
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityEvent, UMyAbilityInstance*);
    FOnAbilityEvent m_OnActivated, m_OnEnded, m_OnCanceled;

    // Accessors
    UFUNCTION(BlueprintPure) 
    const UAbilityComponent* GetAbilityComponent() const { return m_pAbilityComp.Get(); }
    UFUNCTION(BlueprintPure)
    const UMyAbilityData* GetAbilityData() const { return m_pAbilityData.Get(); }

    UFUNCTION(BlueprintPure) 
    FAbilityHandle GetHandle() const { return m_Handle; }

private:
    UPROPERTY() 
    TWeakObjectPtr<UAbilityComponent> m_pAbilityComp = nullptr;
    UPROPERTY() 
    TWeakObjectPtr<UMyAbilityData> m_pAbilityData = nullptr;
    UPROPERTY() 
    TArray<UMyAbilityTask*> m_Tasks;
    UPROPERTY() 
    FGameplayTagContainer m_AppliedTags;
    FAbilityHandle m_Handle;
    EAbilityState m_AbilityState = EAbilityState::Spawning;

	uint8 m_bHasEnded :1;
};
