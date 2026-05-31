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

protected:
	void ChangeMouseMove();
protected:
	TWeakObjectPtr<APlayerController> m_PlayerController;
	FDelegateHandle m_ChangeMouseMoveHandle;
};
