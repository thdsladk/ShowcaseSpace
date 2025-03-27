// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
//#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()

{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	m_BaseStat.Level = 1;
	m_BaseStat.AttackRange = 100;
	m_BaseStat.AttackRadius = 150;

}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	/* 이시점에 초기화 해주어야 PostInitializeComponents보다 먼저 호출되어서 문제가 없다.
		스텟이 제대로 초기화 되기 전에 호출하면 값이 잘못 호출되는 문제가 있었다 .
		HP와 MaxHp가 0.0이 되어서 0.0/0.0을 해버리는 문제 생겼다. ( Log... ) 
		!!! 나누기는 분모가 0이 되는 경우를 주의하자 !!! 수가 무한으로 수렴해 버린다.. 
	*/
	
	SetLevel(m_BaseStat.Level);
}

void UMyStatComponent::SetLevel(int32 Level)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance != nullptr)
	{
		// Base Setting
		auto StatData = MyGameInstance->GetStatData((Level));
		if (StatData != nullptr)
		{
			m_BaseStat.Level	 = 	StatData->Level;
			m_BaseStat.HP		 = 	StatData->HP;
			m_BaseStat.MP		 = 	StatData->MP;
			m_BaseStat.SP		 = 	StatData->SP;

			m_BaseStat.Attack	 = 	StatData->Attack;
			m_BaseStat.Defence	 = 	StatData->Defence;
			m_BaseStat.Speed	 = 	StatData->Speed;
			m_BaseStat.WillPower = 	StatData->WillPower;

			m_BaseStat.AttackRange = StatData->AttackRange;
			m_BaseStat.AttackRadius = StatData->AttackRadius;
			

		}
		// Gauge Setting
		m_CurrentStat.MaxHP		= m_BaseStat.HP + m_ModifierStat.HP;
		m_CurrentStat.MaxMP		= m_BaseStat.MP + m_ModifierStat.MP;
		m_CurrentStat.MaxSP		= m_BaseStat.SP + m_ModifierStat.SP;
		if (m_CurrentStat.HP <= 0) { m_CurrentStat.HP = m_CurrentStat.MaxHP; }
		if (m_CurrentStat.MP <= 0) { m_CurrentStat.MP = m_CurrentStat.MaxMP; }
		if (m_CurrentStat.SP <= 0) { m_CurrentStat.SP = m_CurrentStat.MaxSP; }

		// Notify
		OnHPChanged.Broadcast();
		OnMPChanged.Broadcast();
		OnSPChanged.Broadcast();
		OnStatChanged.Broadcast(m_BaseStat, m_ModifierStat);
	}
}

void UMyStatComponent::SetBaseAttack(int32 Attack)
{
	m_BaseStat.Attack = FMath::Clamp(Attack, 0, MaxStatus);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetModifierAttack(int32 Attack)
{
	m_ModifierStat.Attack = FMath::Clamp(Attack, 0, MaxStatus);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetBaseDefence(int32 Defence)
{
	m_BaseStat.Defence = FMath::Clamp(Defence, 0, MaxStatus);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetModifierDefence(int32 Defence)
{
	m_ModifierStat.Defence = FMath::Clamp(Defence, 0, MaxStatus);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}
/// <summary>
/// HP 비율 반환
/// </summary>
/// <returns>음수면 실패</returns>
const float UMyStatComponent::GetHPRatio()
{ 
	if (m_CurrentStat.MaxHP == 0)
		return -1.f;
	return ((m_CurrentStat.HP) / static_cast<float>(m_CurrentStat.MaxHP)); 
}
/// <summary>
/// MP 비율 반환
/// </summary>
/// <returns>음수면 실패</returns>
const float UMyStatComponent::GetMPRatio()
{
	if (m_CurrentStat.MaxMP == 0)
		return -1.f;
	return ((m_CurrentStat.MP) / static_cast<float>(m_CurrentStat.MaxMP));
}
/// <summary>
/// SP 비율 반환
/// </summary>
/// <returns>음수면 실패</returns>
const float UMyStatComponent::GetSPRatio()
{
	if (m_CurrentStat.MaxSP == 0)
		return -1.f;
	return ((m_CurrentStat.SP) / static_cast<float>(m_CurrentStat.MaxSP));
}

void UMyStatComponent::SetHP(int32 HP)
{
	m_CurrentStat.HP = FMath::Clamp(HP, 0, m_CurrentStat.MaxHP);

	OnHPChanged.Broadcast();

	if (m_CurrentStat.HP == 0)
	{
		OnDeathCheck.Broadcast();
	}

	if (m_CurrentStat.HP == m_CurrentStat.MaxHP)
	{
		if (m_StatRecovery[0] > 0) { m_StatRecovery[0] *= -1; }
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_HPRecovery);
	}
	else if (m_CurrentStat.HP <= m_CurrentStat.MaxHP)
	{
		if (m_StatRecovery[0] < 0)
		{
			m_StatRecovery[0] *= -1;
			GetWorld()->GetTimerManager().SetTimer(m_Timer_HPRecovery, this, &UMyStatComponent::Recovery_HP, 3.f, true);
		}
	}
}

void UMyStatComponent::SetMP(int32 MP)
{
	m_CurrentStat.MP = FMath::Clamp(MP, 0, m_CurrentStat.MaxMP);

	OnMPChanged.Broadcast();

	if (m_CurrentStat.MP == m_CurrentStat.MaxMP)
	{
		if (m_StatRecovery[1] > 0) { m_StatRecovery[1] *= -1; }
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_MPRecovery);
	}
	else if (m_CurrentStat.MP <= m_CurrentStat.MaxMP)
	{
		if (m_StatRecovery[1] < 0)
		{
			m_StatRecovery[1] *= -1;
			GetWorld()->GetTimerManager().SetTimer(m_Timer_MPRecovery, this, &UMyStatComponent::Recovery_MP, 3.f, true);
		}
	}

}

void UMyStatComponent::SetSP(int32 SP)
{
	m_CurrentStat.SP = FMath::Clamp(SP, 0, m_CurrentStat.MaxSP);


	OnSPChanged.Broadcast();

	if (m_CurrentStat.SP == m_CurrentStat.MaxSP)
	{
		if (m_StatRecovery[2] > 0) { m_StatRecovery[2] *= -1; }
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_SPRecovery);
	}
	else if (m_CurrentStat.SP <= m_CurrentStat.MaxSP)
	{
		if (m_StatRecovery[2] < 0)
		{
			m_StatRecovery[2] *= -1;
			GetWorld()->GetTimerManager().SetTimer(m_Timer_SPRecovery, this, &UMyStatComponent::Recovery_SP, 3.f, true);
		}
	}


}

void UMyStatComponent::OnAttacked(int32 DamageAmount)
{
	SetHP(m_CurrentStat.HP - DamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), m_CurrentStat.HP);
}

void UMyStatComponent::AddBaseStat(const FBaseStatusData& InAddBaseStat)
{
	m_BaseStat = (m_BaseStat + InAddBaseStat); 
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetBaseStat(const FBaseStatusData& InSetBaseStat)
{
	m_BaseStat = InSetBaseStat;  
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::AddModifierStat(const FBaseStatusData& InAddModifierStat)
{
	m_ModifierStat = (m_ModifierStat + InAddModifierStat);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetModifierStat(const FBaseStatusData& InSetModifierStat)
{
	m_ModifierStat = InSetModifierStat;
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::Recovery_HP()
{
	SetHP((m_CurrentStat.HP) + m_StatRecovery[0]);
	UE_LOG(LogTemp, Warning, TEXT("Recovery_HP %d"), m_CurrentStat.HP);
}

void UMyStatComponent::Recovery_MP()
{
	SetMP((m_CurrentStat.MP) + m_StatRecovery[1]);
	UE_LOG(LogTemp, Warning, TEXT("Recovery_MP %d"), m_CurrentStat.MP);
}

void UMyStatComponent::Recovery_SP()
{
	SetSP((m_CurrentStat.SP) + m_StatRecovery[2]);
	UE_LOG(LogTemp, Warning, TEXT("Recovery_SP %d"), m_CurrentStat.SP);
}

