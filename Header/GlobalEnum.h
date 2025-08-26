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

/* ������ �Ǵ� ��ų ȿ���� ���� */
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
	Self		  UMETA(DisplayName = "Self"),			// �ڽŸ�
	Ally          UMETA(DisplayName = "Ally"),			// �Ʊ��� [ �߸��� ���� ]
	SelfAndAlly	  UMETA(DisplayName = "SelfAndAlly"),	// �ڽ� , �Ʊ�
	Enermy		  UMETA(DisplayName = "Enermy"),		// ����
	Other		  UMETA(DisplayName = "Other"),			// �� , �Ʊ� ��� 
	EveryOne      UMETA(DisplayName = "EveryOne"),		// ���  [ EveryBody �� ��� ]
	End           UMETA(DisplayName = "End")
};


UENUM(BlueprintType)
enum class ESkillApplicationType : uint8
{
	Instant       UMETA(DisplayName = "Instant"),      // �ٷ� �� �� ����
	Channeled     UMETA(DisplayName = "Channeled"),    // ä�θ��ϴ� ���� ƽ���� ����
	OverTime      UMETA(DisplayName = "OverTime"),     // DoT (���� ����)
	Burst         UMETA(DisplayName = "Burst"),        // ª�� ����/�ð� �� �ݺ� ����
	Hybrid        UMETA(DisplayName = "Hybrid"),       // �� ����� ������ ����
	End           UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	// �� ������ ������
	Slashing				UMETA(DisplayName = "Slashing"),		// [ü�� ������]
	Piercing				UMETA(DisplayName = "Piercing"),		// [ü�� ������]
	Blunt					UMETA(DisplayName = "Blunt"),			// [ü�� ������] 

	// �� ����(������Ż) ������
	Fire					UMETA(DisplayName = "Fire"),			// [ü�� ������]
	Ice						UMETA(DisplayName = "Ice"),				// [ü�� ������]
	Lightning				UMETA(DisplayName = "Lightning"),		// [ü�� ������] [���� ������]
	Earth					UMETA(DisplayName = "Earth"),			// [ü�� ������]
	Water					UMETA(DisplayName = "Water"),			// [ü�� ������]
	Wind					UMETA(DisplayName = "Wind"),			// [ü�� ������]

	// �� ����/���� �迭 ������
	Holy					UMETA(DisplayName = "Holy"),			// [����,�����/���Ǻ�] [���� ������] [ü�� ������]
	Dark					UMETA(DisplayName = "Dark"),			// [����,�����/���Ǻ�] [���� ������] [ü�� ������]
	Chaos					UMETA(DisplayName = "Chaos"),			// [���� ������] [��� ������] [�����]
	Cursed					UMETA(DisplayName = "Cursed"),			// [���� ������] �Ǵ� [�����]

	// �� ȭ��/�ν� �� �� �迭 ������
	Acid					UMETA(DisplayName = "Acid"),			// [ü�� ������] [���� ������]
	Poison					UMETA(DisplayName = "Poison"),			// [ü�� ������] [�����]
	Corrosive				UMETA(DisplayName = "Corrosive"),		// ���ݿ� ������ �ش�. [�����]

	// �� ����/������ �迭 ������
	Psychic					UMETA(DisplayName = "Psychic"),			// [���� ������] [��� ������] [�����]
	Sonic					UMETA(DisplayName = "Sonic"),			// [�����]
	Force					UMETA(DisplayName = "Force"),			// ���� ������ ������ ��� ����
	Gravity					UMETA(DisplayName = "Gravity"),
	Temporal				UMETA(DisplayName = "Temporal"),		// ȸ���� ���ҵ� ���ȿ� ������ �ش�. [�����]

	// End (���� Enum ������ �׸�)
	End						UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EHitType : uint8
{
	HitMiss			 UMETA(DisplayName = "Miss Hit"),		 // ������
	HitNormal        UMETA(DisplayName = "Normal Hit"),		 // ��Ÿ
	HitCritical      UMETA(DisplayName = "Critical Hit"),    // ġ��Ÿ
	HitDoubleStrike  UMETA(DisplayName = "Double Strike"),   // ����Ÿ
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


