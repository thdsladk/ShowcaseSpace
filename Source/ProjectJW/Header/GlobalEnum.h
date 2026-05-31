#pragma once

#include "Header/EquipmentEnum.h"

namespace Combat
{

}

namespace Ability
{

}

UENUM()
enum class EJWCollisionShapeType : uint8
{
	Line	UMETA(DisplayName = "Line"),
	Box		UMETA(DisplayName = "Box"),
	Sphere	UMETA(DisplayName = "Sphere"),
	Capsule	UMETA(DisplayName = "Capsule"),
	End
};

UENUM()
enum class ECombatMode : uint8
{
	IdleMode		UMETA(DisplayName = "IdleMode"),
	MeleeMode		UMETA(DisplayName = "MeleeMode"),
	RangeMode		UMETA(DisplayName = "RangeMode"),
	DualBladeMode	UMETA(DisplayName = "DualBladeMode"),
	End
};


// uint8 로 해야 데이터를 보낼때 문제가 안생긴다. 단 128라는 갯수 제한이 있다...
UENUM()
enum class EInputAbilityID : uint8
{
	// Ability
	Jump 					UMETA(DisplayName = "Jump"),
	Attack					UMETA(DisplayName = "Attack"),
	Skill					UMETA(DisplayName = "Skill"),
	Sprint					UMETA(DisplayName = "Sprint"),
	Dodge					UMETA(DisplayName = "Dodge"),
	ChangeCombatMode		UMETA(DisplayName = "ChangeCombatMode"),
	RangeAttack				UMETA(DisplayName = "RangeAttack"),
	Defense					UMETA(DisplayName = "Defense"),
	SkillQ					UMETA(DisplayName = "SkillQ"),
	SkillE					UMETA(DisplayName = "SkillE"),
	SkillR					UMETA(DisplayName = "SkillR"),
	TargetOn				UMETA(DisplayName = "TargetOn"),
	DashAttack				UMETA(DisplayName = "DashAttack"),
	Confirm					UMETA(DisplayName = "Confirm"),
	Cancel					UMETA(DisplayName = "Cancel"),
	End						UMETA(DisplayName = "End")
};

UENUM()
enum class EInputID : uint8
{
	ChangeControl			UMETA(DisplayName = "ChangeControl"),
	ToggleMenu				UMETA(DisplayName = "ToggleMenu"),
	End						UMETA(DisplayName = "End")
};

UENUM()
enum class EJWSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EJWConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EJWValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EAttributeType : uint8
{
	CharacterAttribute			UMETA(DisplayName = "CharacterAttribute"),
	CharacterSkillAttribute		UMETA(DisplayName = "CharacterSkillAttribute"),
	CharacterWeaponAttribute	UMETA(DisplayName = "CharacterWeaponAttribute"),
	End							UMETA(DisplayName = "End")
};

UENUM()
enum class ETargetingMode : uint8
{
	LineTrace				UMETA(DisplayName = "LineTrace"),
	Trace					UMETA(DisplayName = "Trace"),
	Radius					UMETA(DisplayName = "Radius"),
	Cursor					UMETA(DisplayName = "Cursor"),
	End						UMETA(DisplayName = "End")
};

UENUM()
enum class ETargetStartPointType : uint8
{
	Character				UMETA(DisplayName = "Character"),
	WeaponSocket			UMETA(DisplayName = "WeaponSocket"),
	Cursor					UMETA(DisplayName = "Cursor"),
	End						UMETA(DisplayName = "End")
};

UENUM()
enum class ETargetType : uint8
{
	Self					UMETA(DisplayName = "Self"),
	Friendly				UMETA(DisplayName = "Friendly"),
	Hostile					UMETA(DisplayName = "Hostile"),
	Natural					UMETA(DisplayName = "Natural"),
	End						UMETA(DisplayName = "End")
};


// 참고 사항
/*

네임 스페이스를 사용해서 정의하면 아래 문법으로 열거형의 이름값을 생략해서 사용할 수 있다. 

using ECombatMode;
using EEquipmentType;
using EJWSuccessType;
using EJWConfirmType;
using EJWValidType;



*/