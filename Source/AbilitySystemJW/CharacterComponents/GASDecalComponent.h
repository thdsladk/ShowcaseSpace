// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "GASDecalComponent.generated.h"

class AAOEBase;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASDecalComponent : public UDecalComponent
{
	GENERATED_BODY()
public:
	UGASDecalComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddAOEBase();

	UFUNCTION(BlueprintCallable, Category = "Decal")
	void SetVisibleAOEBase(int32 Index, bool IsVisible);


protected:
	UPROPERTY(EditAnywhere,Category = Decal)
	TSubclassOf<AAOEBase> m_AOEClass;

	TArray<TObjectPtr<AAOEBase>> m_AOEBase;
	TArray<bool> m_IsVisibleAOEBase;

};
