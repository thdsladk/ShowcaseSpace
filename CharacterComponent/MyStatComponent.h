// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/DataStruct.h"
#include "MyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChanged,float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMPChanged,float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSPChanged,float);
DECLARE_MULTICAST_DELEGATE(FOnDeathCheck);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, const FBaseStatusData& BaseStat, const FBaseStatusData& ModifierStat);




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();
	virtual void SetupData(FString TableRawName);

protected:
	virtual void InitializeComponent() override;

#pragma region Status Section
public:	
	// Status Section
	FORCEINLINE const int32 GetLevel() { return m_BaseStat.Level; }
	FORCEINLINE const int32 GetAttack() { return (m_BaseStat.Attack + m_ModifierStat.Attack); }
	FORCEINLINE const float GetAttackSpeed() { return (m_BaseStat.AttackSpeed + m_ModifierStat.AttackSpeed); }
	FORCEINLINE const FBaseStatusData& GetBaseStat() { return m_BaseStat; };
	FORCEINLINE	const FBaseStatusData& GetModifierStat() { return m_ModifierStat; }
	FORCEINLINE FBaseStatusData GetTotalStat() { return m_BaseStat + m_ModifierStat; }

	void SetLevel(int32 Level);
	void SetBaseAttack (int32 Attack);
	void SetModifierAttack(int32 Attack);
	void SetBaseDefence(int32 Defence);
	void SetModifierDefence(int32 Defence);
	void SetBaseAttackSpeed(float Rate);
	void SetModifierAttackSpeed(float Rate);

	FORCEINLINE void AddLevel(int32 Level) { SetLevel(m_BaseStat.Level + Level); }
	FORCEINLINE void AddBaseAttack(int32 Attack) { SetBaseAttack(m_BaseStat.Attack + Attack); }
	FORCEINLINE void AddModifierAttack(int32 Attack) { SetModifierAttack(m_ModifierStat.Attack + Attack); }
	FORCEINLINE void AddBaseDefence(int32 Defence) { SetBaseDefence(m_BaseStat.Defence + Defence); }
	FORCEINLINE void AddModifierDefence(int32 Defence) { SetModifierDefence(m_ModifierStat.Defence + Defence); }
	FORCEINLINE void AddBaseAttackSpeed(float Rate) { SetBaseAttackSpeed(m_BaseStat.AttackSpeed + Rate);}
	FORCEINLINE void AddModifierAttackSpeed(float Rate) { SetModifierAttackSpeed(m_ModifierStat.AttackSpeed + Rate); }

	void AddBaseStat(const FBaseStatusData& InAddBaseStat); 
	void SetBaseStat(const FBaseStatusData& InSetBaseStat); 
	void AddModifierStat(const FBaseStatusData& InAddModifierStat);
	void SetModifierStat(const FBaseStatusData& InSetModifierStat);
#pragma endregion

#pragma region Gauge Section
public:
	// Gauge Section 
	FORCEINLINE const int32& GetHP() { return m_CurrentGauge.HP; }
	FORCEINLINE const int32& GetMP() { return m_CurrentGauge.MP; }
	FORCEINLINE const int32& GetSP() { return m_CurrentGauge.SP; }
	const float GetHPRatio();
	const float GetMPRatio();
	const float GetSPRatio();
	
	void SetHP(int32 HP);
	void SetMP(int32 MP);
	void SetSP(int32 SP);
	void AddHP(int32 HP);
	void AddMP(int32 MP);
	void AddSP(int32 SP);
	// Spend Gauge
	FORCEINLINE void OnAttacked(int32 Damage) { AddHP(-Damage); }
	FORCEINLINE void SpendMP(int32 MP) { AddMP(-MP); }
	FORCEINLINE void SpendSP(int32 SP) { AddSP(-SP); }

public:
	bool CanPay(TMap<EGauge, int32>& Costs);
#pragma endregion

#pragma region Recovery Section
protected:
	void Recovery_HP();
	void Recovery_MP();
	void Recovery_SP();
#pragma endregion

protected:
	// 두 스탯을 합치면 최대치 MaxStat ( Base + Modifier / 고정적인 수치 ) 
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FBaseStatusData m_BaseStat;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FBaseStatusData m_ModifierStat;
	
	// 현재 상태를 나타내는 지수		( 유동적인 수치 )
	UPROPERTY(EditAnywhere, Category = Gauge, meta = (AllowPrivateAccess = true))
	FStatusGaugeData m_CurrentGauge;

public:
	FOnHPChanged OnHPChanged;
	FOnMPChanged OnMPChanged;
	FOnSPChanged OnSPChanged;
	FOnDeathCheck OnDeathCheck;
	FOnStatChanged OnStatChanged;

protected:
	// timer
	FTimerHandle m_Timer_HPRecovery;
	FTimerHandle m_Timer_MPRecovery;
	FTimerHandle m_Timer_SPRecovery;



};

