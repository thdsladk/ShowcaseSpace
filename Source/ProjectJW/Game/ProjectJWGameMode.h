// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectJWGameMode.generated.h"

UCLASS()
class PROJECTJW_API AProjectJWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectJWGameMode();

	virtual void StartPlay() override;


public:
	FORCEINLINE bool IsDebugMode() const { return m_IsDebugMode; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	uint8 m_IsDebugMode : 1;
};



