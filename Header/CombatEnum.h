// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CombatEnum.generated.h"



UENUM(BlueprintType)
enum class ECombatType : uint8
{
	CT_None				UMETA(DisplayName = "None"),
	CT_Melee			UMETA(DisplayName = "Melee"),
	CT_Ranged			UMETA(DisplayName = "Ranged"),
	CT_Magic			UMETA(DisplayName = "Magic"),
	CT_Siege			UMETA(DisplayName = "Siege"),
	CT_Mounted			UMETA(DisplayName = "Mounted"),
	CT_AOE				UMETA(DisplayName = "AOE"),			// Area Of Effect
	CT_End				UMETA(DisplayName = "End"),
};
