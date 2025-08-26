// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "SkillEnum.generated.h"

/*
	
	스킬 관련 enum을 분리 시켜놨다. 
	
*/


UENUM(BlueprintType)
enum class ESKillCategory : uint8
{
	PassiveSkills			UMETA(DisplayName = "PassiveSkills"),
	OffensiveSkills			UMETA(DisplayName = "OffensiveSkills"),
	DefensiveSkills			UMETA(DisplayName = "DefensiveSkills"),
	HealingSkills			UMETA(DisplayName = "HealingSkills"),
	BuffSkills				UMETA(DisplayName = "BuffSkills"),
	DebuffSkills			UMETA(DisplayName = "DebuffSkills"),
	CrowdControlSkills		UMETA(DisplayName = "CrowdControlSkills"),
	MobilitySkills			UMETA(DisplayName = "MobilitySkills"),
	UtilitySkills			UMETA(DisplayName = "UtilitySkills"),
	End						UMETA(DisplayName = "End")

};

UENUM(BlueprintType)
enum class ESKillUIState : uint8
{
	Enable				UMETA(DisplayName = "Enable"),
	Disable				UMETA(DisplayName = "Disable"),
	Hover				UMETA(DisplayName = "Hover"),
	Press				UMETA(DisplayName = "Press"),
	Release				UMETA(DisplayName = "Release"),
	End					UMETA(DisplayName = "End")

};

UENUM(BlueprintType)
enum class ESkill : uint8
{
	Skill_Q				UMETA(DisplayName = "Skill_Q"),
	Skill_W				UMETA(DisplayName = "Skill_W"),
	Skill_E				UMETA(DisplayName = "Skill_E"),
	Skill_R				UMETA(DisplayName = "Skill_R"),
	End					UMETA(DisplayName = "End")

};