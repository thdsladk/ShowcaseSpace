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
 *	게임 데이터들을 관리하는 서브 시스템
 * 
 * 1. 게임 데이터 테이블 들을 호출하자.
 * 2. UObject 또는 AActor를 베이스로 하는 객체 풀을 생성해서 관리하자
 * [ 세분화 한다면 , 게임 오브젝트 풀 / 아이템 풀 / 일회성 오브젝트 풀( ex:화살,발사체 )

 */

 // 비트단위로 Flag를 해서 쓸때 사용할려고 ...
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
	// 엔진이 이 서브시스템을 생성할 때 호출하는 초기화 함수
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 엔진이 서브시스템을 파괴하기 전에 호출하는 종료 함수
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

	// 클래스만 먼저 정의하고 생성해서 쓰는 방식 
	// WidgetComponent로 붙이는 방식과 다르게 사용  
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> m_DamageFont;
#pragma endregion

};
