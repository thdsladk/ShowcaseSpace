// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"
#include "Data/MyGameSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMonsterStatComponent::UMonsterStatComponent()
	
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
	m_BaseStat.Level = 1;
}


// Called when the game starts
void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMonsterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void UMonsterStatComponent::SetupData(FString TableRawName)
{
	UMyGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (GameSubsystem != nullptr)
	{
		// Base Setting
		auto StatData = GameSubsystem->GetNPCStatData(TableRawName);
		if (StatData != nullptr)
		{
			m_BaseStat.Level = StatData->Level;
			m_BaseStat.HP = StatData->HP;
			m_BaseStat.MP = StatData->MP;
			m_BaseStat.SP = StatData->SP;

			m_BaseStat.Attack = StatData->Attack;
			m_BaseStat.Defence = StatData->Defence;
			m_BaseStat.Speed = StatData->Speed;
			m_BaseStat.WillPower = StatData->WillPower;

			m_BaseStat.AttackRange = StatData->AttackRange;
			m_BaseStat.AttackRadius = StatData->AttackRadius;


		}
		// Gauge Setting
		m_CurrentGauge.MaxHP = m_BaseStat.HP + m_ModifierStat.HP;
		m_CurrentGauge.MaxMP = m_BaseStat.MP + m_ModifierStat.MP;
		m_CurrentGauge.MaxSP = m_BaseStat.SP + m_ModifierStat.SP;
		if (m_CurrentGauge.HP <= 0) { m_CurrentGauge.HP = m_CurrentGauge.MaxHP; }
		if (m_CurrentGauge.MP <= 0) { m_CurrentGauge.MP = m_CurrentGauge.MaxMP; }
		if (m_CurrentGauge.SP <= 0) { m_CurrentGauge.SP = m_CurrentGauge.MaxSP; }

		// Notify
		OnHPChanged.Broadcast(GetHPRatio());
		OnMPChanged.Broadcast(GetMPRatio());
		OnSPChanged.Broadcast(GetSPRatio());
		OnStatChanged.Broadcast(m_BaseStat, m_ModifierStat);
	}

}

