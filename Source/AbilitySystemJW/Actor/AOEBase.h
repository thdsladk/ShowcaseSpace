// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "AOEBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AAOEBase : public ADecalActor
{
	GENERATED_BODY()
public:
	AAOEBase();
	virtual void BeginPlay()override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	virtual void Tick(float DeltaTime) override;

	void SetScope(float Range) { m_ScopeRange = Range; }

	uint8 GetDecalColor();
	void SetDecalColor_Red();
	void SetDecalColor_Green();
protected:
	void ChangeMouseMove();

protected:
	TWeakObjectPtr<APlayerController> m_PlayerController;
	FDelegateHandle m_ChangeMouseMoveHandle;

	TObjectPtr<UMaterialInstanceDynamic> m_DynMat;

	UPROPERTY(EditAnywhere,Category = "Tracking")
	bool IsMouseTracking = false;						// false 면 Character Tracking을 하도록 하자. 

	float m_ScopeRange = 0.f;
	uint8 m_CurrentColorIndex = 2U; // 0: Green, 1: Red, 2: Unknown
};
