// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Engine/DataTable.h"
#include "Header/GlobalEnum.h"

#include "DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FStatVector
{
	GENERATED_BODY()

	// �����Ϳ� HP/MP/SP �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	double HP = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	double MP = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	double SP = 0.0;

	// �⺻ ������
	FStatVector() {}
	// FVector �� FStatVector ��ȯ
	FStatVector(const FVector& In)
		: HP(In.X), MP(In.Y), SP(In.Z)
	{}
	operator FVector() const
	{
		return FVector(HP, MP, SP);
	}
};

USTRUCT()
struct FBaseStatusData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WillPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRadius;


	//int32 Zeal;	// ���� ��ġ : ������ �ϰų� ���𰡸� �Ҷ����� �ö󰣴�.

	FBaseStatusData operator+(const FBaseStatusData& Other) const
	{
		const int32* const ThisPtr = reinterpret_cast<const	 int32* const>(this);
		const int32* const OtherPtr = reinterpret_cast<const int32* const>(&Other);

		FBaseStatusData Result;
		int32* ResultPtr = reinterpret_cast<int32*>(&Result);
		int32 StatNum = sizeof(FBaseStatusData) / sizeof(int32);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
	FBaseStatusData operator-() const
	{
		const int32* const ThisPtr = reinterpret_cast<const	 int32* const>(this);

		FBaseStatusData Result;
		int32* ResultPtr = reinterpret_cast<int32*>(&Result);
		int32 StatNum = sizeof(FBaseStatusData) / sizeof(int32);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = -ThisPtr[i];
		}

		return Result;
	}
};

USTRUCT(BlueprintType)
struct FStatusGaugeData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSP;
};

USTRUCT(BlueprintType)
struct FNPCData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level;

	//Gauge Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SP;

	//Status Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WillPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LHItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RHItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LFItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RFItemID;


};

USTRUCT(BlueprintType)
struct FEquipmentInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LHItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RHItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LFItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RFItemID;
};

USTRUCT(BlueprintType)
struct FCharacterInfo : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType Type;

	//Status Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseStatusData Stat;

	//Mesh Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	//Equipment Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipmentInfo EquipmentInfo;

};

#pragma region Item Struct
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Type;									// EItemType�� ��ü�� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEquipmentPart EquipmentPart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseStatusData Status;
	// TSoftObjectPtr�� ���ؼ� ���� ���Ѿ��� ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> EquipmentMesh;
};

// Ȱ��ȭ �� ������ ������ // ������ ���� 
USTRUCT(BlueprintType)
struct FItemRuntimeData  
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemStaticData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Count;

};

// �׳� �ϴ� ���� �������� ������ �� ��� ����. 
USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemStaticData;
	// Data in Inventory 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 IsEquip : 1;

};


#pragma endregion

/* �������� ��ų��(��ų ����)�� ����� �̰� �����ؼ� ���� ��ų�� ����� ������ ¥����. */
USTRUCT(BlueprintType)
struct FSkillElementData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillApplicationType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double CoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Charge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShapeType AreaOfEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double CastTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double EffectTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double OverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double ChannelingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double AnimationLockTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bInterrupt : 1 ;								// �ܺ� ���ο� ���� �ߴ� ����  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EffectTagList;								// Buff,Debuff�� ���� TAG���� ����Ʈ�� ������ "Buff,Debuff"�̷��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ResourceList;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//ETargetType TargetType;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 TargetCount ;

};
/*
*	���̸��� ���� �ϳİ� �߿�  Ticker Symbol : ��ƾ�� �ɺ��� ���������� �ֽ�ó�� ?
		
	ID 
	Type
	Name
	Description
	Icon
	Cost					[FVector�� ����� ���ʿ��ϸ� Cost(���) ����ü�� ��������] 
	CoolDown
	Charge
	Range
	Radius
	AreaOfEffect
	Damage								// Min,Mid,Max�� ���� ������.
	DamageType
	DamageScale							// �̰� �뷫 20%�� ����.

	CastTime
	EffectTime
	OverTime
	ChannelingTime
	AnimationLockTime
	
	SkillApplicationType
	Interrupt

	EffectTagList							// Buff,Debuff Effect  
	ResourceList							// VFS,SFX,AnimationData,CameraEffect

	UIFeedback								// UI�� ������ ���ؼ� �˷�����ϴµ�... 

*/

// �̰� ���������� ���Ǵ� ���� ��ų�� ���� ���̺�
USTRUCT(BlueprintType)
struct FSkillData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double	CoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector	Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8	Charge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8	bInterrupt : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class ASkillCommandBase> SkillClass;

};

USTRUCT(BlueprintType)
struct FCommandData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERadiusType RadiusType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FSkillUIData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChargeCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double CoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 UIState;
};


#pragma region Projectile Data

USTRUCT(BlueprintType)
struct FProjectileData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProjectileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ProjectileType; // EProjectileType ���� ��ü�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double GravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double LifeTime;

	
};


USTRUCT(BlueprintType)
struct FProjectileRunTimeData : public  FProjectileData
{
	GENERATED_BODY()

	// ������ ���̺����� ��� ���� �ʴ� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Owner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Rotation;

};


#pragma endregion


#pragma region Character Info
/*
	ĳ���Ϳ� ���� �������� �����ϴ� ����ü
	ĳ���� �̸� 
	ĳ���� ����? 

	���� ���� / ���� ���� / ���� / ���� 

*/

struct FCombatInfo
{
public:
	const int32 CostDefense_SP = 10;
	const int32 CostAttack_SP = 0;
	const int32 CostSprint_SP = 1;
	const int32 TakeDamage_Aggro = 10;
};

#pragma endregion

#pragma region Ability Info
/* ����Ǿ� ������ ���� . 
struct FGameplayTag
{
private:
	FName TagName;       // "Ability.Attack.Light"
	int32 TagIndex;      // ���� ������Ʈ�� �ε���
public:
	static FGameplayTag RequestGameplayTag(FName TagName);
	bool MatchesTag(FGameplayTag Other) const;
	bool MatchesTagExact(FGameplayTag Other) const;
	FName GetTagName() const { return TagName; }
};

struct FGameplayTagContainer
{
private:
	TArray<FGameplayTag> GameplayTags; // ���� ���� �±�
	TArray<FGameplayTag> ParentTags;   // ���� ���� �±� ĳ��
public:
	bool HasTag(FGameplayTag Tag, bool bExact) const;
	bool HasAny(const FGameplayTagContainer& Other, bool bExact) const;
	bool HasAll(const FGameplayTagContainer& Other, bool bExact) const;
	void AddTag(FGameplayTag Tag);
	void RemoveTag(FGameplayTag Tag);
};
*/
#pragma endregion

#pragma region Test Code




#pragma endregion