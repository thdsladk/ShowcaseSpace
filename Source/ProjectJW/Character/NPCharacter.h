// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Engine/StreamableManager.h"
#include "NPCharacter.generated.h"

/**
 * 
 */
UCLASS(config = ProjectJW)
class PROJECTJW_API ANPCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	ANPCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PostInitializeComponents() override;
	virtual void SetDead() override;

protected:
	void NPCMeshLoadCompleted();

};
