// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NPCharacter.h"
#include "AbilitySystemInterface.h"
#include "Interface/PawnCombatInterface.h"
#include "Interface/WidgetInterface.h"
#include "GameplayTagContainer.h"
#include "GASNPCharacter.generated.h"

class UAbilitySystemComponent;
class UCharacterAttributeSet;
class UGameplayEffect;
class UGASWidgetComponent;
class UGASGaugeBarUserWidget;
class UDA_StartupNPCharacter;
class UGameplayAbility;
class UTargetSystemComponent;
class UWidgetSystemComponent;

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AGASNPCharacter : public ANPCharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IWidgetInterface
{
	GENERATED_BODY()
public:
	AGASNPCharacter(const FObjectInitializer& ObjectInitializer);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

#pragma region GetSet Functions
public:
	virtual UTargetSystemComponent* GetTargetSystemComponent() const override{ return m_TargetSystemComponent; }
	virtual UPhysicsMotionSystemComponent* GetPhysicsMotionSystemComponent() const override { return m_PhysicsMotionSystemComponent; }

#pragma endregion

protected:
	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaSeconds);
public:
	UFUNCTION()
	virtual void OnOutOfHealth();

#pragma region PawnCombat Interface
public:
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override { return m_PawnCombatComponent; }
#pragma endregion

#pragma region AbilitySystem Function
public:
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void TryActivateAbility_Tag(FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void TryActivateAbility_Class(TSubclassOf<UGameplayAbility> AbilityClass);

#pragma endregion


#pragma region Widget Interface
public:
	virtual void SetRangeGaugeBar_Implementation(bool bVisible) override {}
	virtual void SetVisibleGaugeBar_Implementation(bool bVisible) override;

#pragma endregion

#pragma region Damage Functions

	virtual void PlayDamageFont(float DamageAmount, FVector HitPoint = FVector::ZeroVector)override;


#pragma endregion



protected:
	UPROPERTY(EditAnywhere, Category = "Components|Ability")
	TObjectPtr<UAbilitySystemComponent> m_AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnCombatComponent> m_PawnCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|TargetSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTargetSystemComponent> m_TargetSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|PhysicsMotionSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsMotionSystemComponent> m_PhysicsMotionSystemComponent;



	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> m_AttributeSet;

	UPROPERTY(EditAnywhere, Category = "GAS|AbilityData")
	TSoftObjectPtr<UDA_StartupNPCharacter> m_StartupNPCharacterData;


	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> m_InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float m_Level;

#pragma region Widget Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|WidgetComp", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGASWidgetComponent> m_GaugeBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|WidgetComp", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGASGaugeBarUserWidget> m_GaugeWidgetClass;

protected:
	UPROPERTY(EditAnywhere, Category = "Widget|Value")
	FVector2D m_GaugeBarToolSize;

#pragma endregion
};
