// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
//#include "GA/GA_PlayerAbility.h"
#include "Header/GlobalEnum.h"
#include "GA_ChangeCombatMode.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_ChangeCombatMode : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ChangeCombatMode();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION()
	void OnCompleteCallback_Sheathe();

	UFUNCTION()
	void OnInterruptedCallback_Sheathe();


	UFUNCTION()
	void OnCompleteCallback_Draw();

	UFUNCTION()
	void OnInterruptedCallback_Draw();

	void ChangeEquipment();
	FName GetNextSection();

protected:
	UPROPERTY(EditAnywhere, Category = "CombatMode")

	TMap<FGameplayTag, ECombatMode> m_CombatModeFromTriggerEventTag;
	UPROPERTY(EditAnywhere, Category = "CombatMode")
	ECombatMode m_CombatMode = ECombatMode::End;
	
	UPROPERTY(EditAnywhere, Category = "CombatMode")
	TArray<FGameplayTag> m_CombatModeTagList;

protected:
	FName m_NextSectionName[2];
	uint8 m_CurrentSectionIndex;

	uint8 m_bEquip : 1;

	//uint8 m_OriginMovementMode;
};
