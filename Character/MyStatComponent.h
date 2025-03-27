// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyGameInstance.h"
#include "MyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_MULTICAST_DELEGATE(FOnMPChanged);
DECLARE_MULTICAST_DELEGATE(FOnSPChanged);
DECLARE_MULTICAST_DELEGATE(FOnDeathCheck);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, const FBaseStatusData& BaseStat, const FBaseStatusData& ModifierStat);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	virtual void InitializeComponent() override;

public:	
	// Status Section
	FORCEINLINE int32 GetLevel() { return m_BaseStat.Level; }
	FORCEINLINE int32 GetAttack() { return (m_BaseStat.Attack + m_ModifierStat.Attack); }
	FORCEINLINE const FBaseStatusData& GetBaseStat() { return m_BaseStat; };
	FORCEINLINE	const FBaseStatusData& GetModifierStat() { return m_ModifierStat; }
	FORCEINLINE FBaseStatusData GetTotalStat() { return m_BaseStat + m_ModifierStat; }

	void SetLevel(int32 Level);
	void SetBaseAttack (int32 Attack);
	void SetModifierAttack(int32 Attack);
	void SetBaseDefence(int32 Defence);
	void SetModifierDefence(int32 Defence);

	FORCEINLINE void AddLevel(int32 Level) { SetLevel(m_BaseStat.Level + Level); }
	FORCEINLINE void AddBaseAttack(int32 Attack) { SetBaseAttack(m_BaseStat.Attack + Attack); }
	FORCEINLINE void AddModifierAttack(int32 Attack) { SetModifierAttack(m_ModifierStat.Attack + Attack); }
	FORCEINLINE void AddBaseDefence(int32 Defence) { SetBaseDefence(m_BaseStat.Defence + Defence); }
	FORCEINLINE void AddModifierDefence(int32 Defence) { SetModifierDefence(m_ModifierStat.Defence + Defence); }


	void AddBaseStat(const FBaseStatusData& InAddBaseStat); 
	void SetBaseStat(const FBaseStatusData& InSetBaseStat); 
	void AddModifierStat(const FBaseStatusData& InAddModifierStat);
	void SetModifierStat(const FBaseStatusData& InSetModifierStat);
	

	// Gauge Section 
	FORCEINLINE const int32& GetHP() { return m_CurrentStat.HP; }
	FORCEINLINE const int32& GetMP() { return m_CurrentStat.MP; }
	FORCEINLINE const int32& GetSP() { return m_CurrentStat.SP; }
	const float GetHPRatio();
	const float GetMPRatio();
	const float GetSPRatio();
	
	void SetHP(int32 HP);
	void SetMP(int32 MP);
	void SetSP(int32 SP);
	FORCEINLINE void AddHP(int32 HP){ SetHP(m_CurrentStat.HP + HP); }
	FORCEINLINE void AddMP(int32 MP){ SetMP(m_CurrentStat.MP + MP); }
	FORCEINLINE void AddSP(int32 SP){ SetSP(m_CurrentStat.SP + SP); }
	void OnAttacked(int32 DamageAmount);



protected:

	void Recovery_HP();
	void Recovery_MP();
	void Recovery_SP();

protected:
	
	// 두 스탯을 합치면 최대치 MaxStat ( Base + Modifier / 고정적인 수치 ) 
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FBaseStatusData m_BaseStat;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FBaseStatusData m_ModifierStat;
	
	// 현재 상태를 나타내는 지수		( 유동적인 수치 )
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FStatusGaugeData m_CurrentStat;


public:

	FOnHPChanged OnHPChanged;
	FOnMPChanged OnMPChanged;
	FOnSPChanged OnSPChanged;
	FOnDeathCheck OnDeathCheck;
	FOnStatChanged OnStatChanged;

	// timer
	int32 m_StatRecovery[3]{-1,-1,-1};
	
	FTimerHandle m_Timer_HPRecovery;
	FTimerHandle m_Timer_MPRecovery;
	FTimerHandle m_Timer_SPRecovery;



};



/*

스텟 자연 회복 시스템은 m_StateRecovery 값이 음수면 비활성화 상태 양수면 활성화상태



*/