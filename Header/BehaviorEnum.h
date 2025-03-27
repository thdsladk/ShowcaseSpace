// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "BehaviorEnum.generated.h"

/*
	
	행동 관련 enum을 분리 시켜놨다. 
	CharacterBase / AnimInstanceBase에서 활용되고 있다. 
	AI에서도 활용 해야한다. 
*/
UENUM(BlueprintType)
enum class EBehaviorState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walk			UMETA(DisplayName = "Walk"),
	Running			UMETA(DisplayName = "Running"),
	Chase			UMETA(DisplayName = "Chase"),
	Attacking		UMETA(DisplayName = "Attacking"),
	Attacking_End	UMETA(DisplayName = "Attacking_End"),
	Hit				UMETA(DisplayName = "Hit"),
	Defense			UMETA(DisplayName = "Defense"),
	Stunned			UMETA(DisplayName = "Stunned"),
	Casting			UMETA(DisplayName = "Casting"),
	Detect			UMETA(DisplayName = "Detect"),
	Battle			UMETA(DisplayName = "Battle"),
	Alert			UMETA(DisplayName = "Alert"),
	Die				UMETA(DisplayName = "Die"),
	End				UMETA(DisplayName = "End")

};
