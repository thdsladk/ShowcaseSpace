// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "BehaviorEnum.generated.h"

/*
	
	�ൿ ���� enum�� �и� ���ѳ���. 
	CharacterBase / AnimInstanceBase���� Ȱ��ǰ� �ִ�. 
	AI������ Ȱ�� �ؾ��Ѵ�. 
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
