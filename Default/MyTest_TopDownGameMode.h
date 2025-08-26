// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/GameInterface.h"
#include "MyTest_TopDownGameMode.generated.h"



UCLASS(minimalapi)
class AMyTest_TopDownGameMode : public AGameModeBase , public IGameInterface
{
	GENERATED_BODY()

public:
	AMyTest_TopDownGameMode();

public:
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override { return m_bIsCleared; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
		int32 m_ClearScore;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
		int32 m_CurrentScore;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
		uint8 m_bIsCleared : 1;

};



