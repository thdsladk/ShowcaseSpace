// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GlobalEnum.generated.h"


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	NonCharacter		UMETA(DisplayName = "NonCharacter"),
	NPCharacter			UMETA(DisplayName = "NPCharacter"),
	PlayerCharacter		UMETA(DisplayName = "PlayerCharacter"),
	End
};


UENUM(BlueprintType)
enum class EItemType : uint8
{
	NullItem			UMETA(DisplayName = "NullItem"),
	ConsumableItem		UMETA(DisplayName = "ConsumableItem"),
	Non_ConsumableItem	UMETA(DisplayName = "Non_ConsumableItem"),
	EquipmentItem		UMETA(DisplayName = "EquipmentItem"),
	EtcItem				UMETA(DisplayName = "EtcItem"),
	End					UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EEquipmentPart : uint8
{
	Head	UMETA(DisplayName = "Head"),
	Hand_L	UMETA(DisplayName = "Hand_L"),
	Chest	UMETA(DisplayName = "Chest"),
	Hand_R	UMETA(DisplayName = "Hand_R"),
	Foot_L	UMETA(DisplayName = "Foot_L"),
	Foot_R	UMETA(DisplayName = "Foot_R"),
	End
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

UENUM(BlueprintType)
enum class EShapeType : uint8
{
	Line			UMETA(DisplayName = "Line"),
	Box				UMETA(DisplayName = "Box"),
	Sphere			UMETA(DisplayName = "Sphere"),
	Capsule			UMETA(DisplayName = "Capsule"),
	End				UMETA(DisplayName = "End")
};

/* 아이템 또는 스킬 효과의 종류 */
UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Instant UMETA(DisplayName = "Instant"),
	prologed UMETA(DisplayName = "prologed"),
	End UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Self		  UMETA(DisplayName = "Self"),			// 자신만
	Ally          UMETA(DisplayName = "Ally"),			// 아군만 [ 중립도 포함 ]
	SelfAndAlly	  UMETA(DisplayName = "SelfAndAlly"),	// 자신 , 아군
	Enermy		  UMETA(DisplayName = "Enermy"),		// 적만
	Other		  UMETA(DisplayName = "Other"),			// 적 , 아군 모두 
	EveryOne      UMETA(DisplayName = "EveryOne"),		// 모두  [ EveryBody 도 고려 ]
	End           UMETA(DisplayName = "End")
};


UENUM(BlueprintType)
enum class ESkillApplicationType : uint8
{
	Instant       UMETA(DisplayName = "Instant"),      // 바로 한 번 적용
	Channeled     UMETA(DisplayName = "Channeled"),    // 채널링하는 동안 틱마다 적용
	OverTime      UMETA(DisplayName = "OverTime"),     // DoT (지속 피해)
	Burst         UMETA(DisplayName = "Burst"),        // 짧은 범위/시간 내 반복 피해
	Hybrid        UMETA(DisplayName = "Hybrid"),       // 위 방식을 조합한 형태
	End           UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	// ① 물리적 데미지
	Slashing				UMETA(DisplayName = "Slashing"),		// [체력 데미지]
	Piercing				UMETA(DisplayName = "Piercing"),		// [체력 데미지]
	Blunt					UMETA(DisplayName = "Blunt"),			// [체력 데미지] 

	// ② 원소(엘리멘탈) 데미지
	Fire					UMETA(DisplayName = "Fire"),			// [체력 데미지]
	Ice						UMETA(DisplayName = "Ice"),				// [체력 데미지]
	Lightning				UMETA(DisplayName = "Lightning"),		// [체력 데미지] [지속 데미지]
	Earth					UMETA(DisplayName = "Earth"),			// [체력 데미지]
	Water					UMETA(DisplayName = "Water"),			// [체력 데미지]
	Wind					UMETA(DisplayName = "Wind"),			// [체력 데미지]

	// ③ 마법/비전 계열 데미지
	Holy					UMETA(DisplayName = "Holy"),			// [버프,디버프/조건부] [마력 데미지] [체력 데미지]
	Dark					UMETA(DisplayName = "Dark"),			// [버프,디버프/조건부] [마력 데미지] [체력 데미지]
	Chaos					UMETA(DisplayName = "Chaos"),			// [마력 데미지] [기력 데미지] [디버프]
	Cursed					UMETA(DisplayName = "Cursed"),			// [지속 데미지] 또는 [디버프]

	// ④ 화학/부식 및 독 계열 데미지
	Acid					UMETA(DisplayName = "Acid"),			// [체력 데미지] [지속 데미지]
	Poison					UMETA(DisplayName = "Poison"),			// [체력 데미지] [디버프]
	Corrosive				UMETA(DisplayName = "Corrosive"),		// 스텟에 영향을 준다. [디버프]

	// ⑤ 정신/에너지 계열 데미지
	Psychic					UMETA(DisplayName = "Psychic"),			// [마력 데미지] [기력 데미지] [디버프]
	Sonic					UMETA(DisplayName = "Sonic"),			// [디버프]
	Force					UMETA(DisplayName = "Force"),			// 순수 에너지 데미지 방어 무시
	Gravity					UMETA(DisplayName = "Gravity"),
	Temporal				UMETA(DisplayName = "Temporal"),		// 회복력 감소등 스탯에 영향을 준다. [디버프]

	// End (보통 Enum 마지막 항목)
	End						UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EHitType : uint8
{
	HitMiss			 UMETA(DisplayName = "Miss Hit"),		 // 빗맞음
	HitNormal        UMETA(DisplayName = "Normal Hit"),		 // 평타
	HitCritical      UMETA(DisplayName = "Critical Hit"),    // 치명타
	HitDoubleStrike  UMETA(DisplayName = "Double Strike"),   // 이중타
	End				 UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EGauge : uint8
{
	HP				UMETA(DisplayName = "HP"),
	MP				UMETA(DisplayName = "MP"),
	SP				UMETA(DisplayName = "SP"),
	End				UMETA(DisplayName = "End")
};


