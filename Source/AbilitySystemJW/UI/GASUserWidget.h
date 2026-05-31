// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "GASUserWidget.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

#pragma region Blueprint GetSet
public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UAbilitySystemComponent* BP_GetAbilitySystemComponent()const { return GetAbilitySystemComponent(); }
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> m_AbilitySystemComponent;

	
};
