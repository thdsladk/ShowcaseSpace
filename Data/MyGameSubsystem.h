// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Header/DataStruct.h"  
#include "MyGameSubsystem.generated.h"

constexpr int32 MaxStatus = 99999;

class UUserWidget;
//class UDataTable;		
//class UMyItemData;




/**
 *	���� �����͵��� �����ϴ� ���� �ý���
 * 
 * 1. ���� ������ ���̺� ���� ȣ������.
 * 2. UObject �Ǵ� AActor�� ���̽��� �ϴ� ��ü Ǯ�� �����ؼ� ��������
 * [ ����ȭ �Ѵٸ� , ���� ������Ʈ Ǯ / ������ Ǯ / ��ȸ�� ������Ʈ Ǯ( ex:ȭ��,�߻�ü )

 */

 // ��Ʈ������ Flag�� �ؼ� ���� ����ҷ��� ...
USTRUCT()
struct FBitPool
{
	GENERATED_BODY()
public:
	uint8 Element : 1;
};

UCLASS()
class MYTEST_TOPDOWN_API UMyGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMyGameSubsystem();
	// ������ �� ����ý����� ������ �� ȣ���ϴ� �ʱ�ȭ �Լ�
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ������ ����ý����� �ı��ϱ� ���� ȣ���ϴ� ���� �Լ�
	virtual void Deinitialize() override;


	// Data Function
	const FCharacterInfo* GetCharacterInfo(FString Name);

	const FBaseStatusData* GetCharacterInfo_StatData(FString Name);
	USkeletalMesh* GetCharacterInfo_MeshData(FString Name);
	const FEquipmentInfo* GetCharacterInfo_EquipmentInfo(FString Name);


	const FBaseStatusData* GetStatData(int32 Level);
	const FNPCData* GetNPCStatData(FString Name);
	const FItemData* GetItemData(FString Name);
	const FItemData* GetItemData(int32 ID);

	const FSkillElementData* GetSkillElementData(FString Name);
	const FSkillData* GetSkillData(int32 ID);
	const FCommandData* GetCommandData(uint8 Command);

	const int32 GetItemTableSize();



#pragma region ObjectPool Function
public:
	// ObjectPool Function
	UUserWidget* GetDamageFont();
	AActor* GetItemActor(FItemRuntimeData ItemRuntimeData,FTransform Transform);

#pragma endregion

protected:
	//TArray<FBaseStatusData> m_CharacterStatTable;

	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_CharacterInfo;
	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_MyStats;
	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_MonsterStats;
	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_ItemTable;
	TArray<FItemData*> m_ItemTable_Index;
	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_SkillElementTable;
	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_CommandTable;
	UPROPERTY()
	TWeakObjectPtr<UDataTable> m_SkillTable;

#pragma region ObjectPool Section
protected:
	// Object Pool Section
	UPROPERTY()
	TArray<TWeakObjectPtr<UObject>> m_GameObjectPool;
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> m_ProjectileActorPool;
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> m_ItemActorPool;
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> m_ParticleActorPool;
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> m_SkillActorPool;

	UPROPERTY()
	TArray<TWeakObjectPtr<UUserWidget>> m_DamageFontPool;

	// Ŭ������ ���� �����ϰ� �����ؼ� ���� ��� 
	// WidgetComponent�� ���̴� ��İ� �ٸ��� ���  
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> m_DamageFont;
#pragma endregion

};
