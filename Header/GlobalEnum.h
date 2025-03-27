// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GlobalEnum.generated.h"

UENUM(BlueprintType)
enum class EBattleCommand : uint8
{
	Null				UMETA(DisplayName = "Null"),
	Attack				UMETA(DisplayName = "Attack"),
	SmashAttack			UMETA(DisplayName = "SmashAttack"),
	Defense				UMETA(DisplayName = "Defense"),
	Cast				UMETA(DisplayName = "Cast"),
	Shout				UMETA(DisplayName = "Shout"),
	FallBack			UMETA(DisplayName = "FallBack"),
	Escape				UMETA(DisplayName = "Escape"),
	End					UMETA(DisplayName = "End"),
	Size = End
};

UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
	None,
	PlusHP,
	PlusMP,
	PlusSP,
	PlusAttack,
	PlusDefence,
	PlusSpeed,
	PlusWillPower,
	End

};

UENUM(BlueprintType)
enum class ERadiusType : uint8
{
	Melee UMETA(DisplayName ="Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
	Detection UMETA(DisplayName = "Detection"),
	Visible UMETA(DisplayName = "Visible"),
	Outside UMETA(DisplayName = "Outside"),
	End UMETA(DisplayName = "End")

};
