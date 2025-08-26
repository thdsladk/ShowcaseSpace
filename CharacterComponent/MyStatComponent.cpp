// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/MyStatComponent.h"
#include "Data/MyGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

/*
	HP,MP,SP 같은 게이지들은 
	Add 함수는 게이지 증감용 ( 내부에 Set함수를 통해서 변화시킨다. ) 
	Set 함수는 값이 직접 변할때 사용 

	따라서 게이지 변화는 Add함수를 통해서 하면된다. 
*/

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	m_BaseStat.Level = 1;

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

void UMyStatComponent::SetupData(FString TableRawName)
{
	UMyGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (GameSubsystem != nullptr)
	{
		// Base Setting
		auto StatData = GameSubsystem->GetCharacterInfo_StatData(TableRawName);
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

#pragma region Status Section
// 레벨에 의한 세팅은 이제 유산이 되었다. 레벨에 따른 스탯 변화는 레벨링 시스템으로 조정하도록 할 예정
// (임시) 이 함수는 변경 되어야 한다. 
void UMyStatComponent::SetLevel(int32 Level)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance != nullptr)
	{
		UMyGameSubsystem* GameSubsystem = GameInstance->GetSubsystem<UMyGameSubsystem>();
		if (GameSubsystem != nullptr)
		{
			// Base Setting
			auto StatData = GameSubsystem->GetStatData((Level));
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

void UMyStatComponent::SetBaseAttackSpeed(float Rate)
{
	// (임시) 최대값을 10.f로 임의로 했다.
	m_BaseStat.AttackSpeed = FMath::Clamp(Rate, 0.f, 10.f);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetModifierAttackSpeed(float Rate)
{
	// (임시) 최대값을 10.f로 임의로 했다.
	m_ModifierStat.AttackSpeed = FMath::Clamp(Rate, 0.f, 10.f);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
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

#pragma endregion

#pragma region Gauge Section
/// <summary>
/// HP 비율 반환
/// </summary>
/// <returns>음수면 실패</returns>
const float UMyStatComponent::GetHPRatio()
{ 
	if (m_CurrentGauge.MaxHP == 0)
		return -1.f;
	return (static_cast<float>(m_CurrentGauge.HP) / static_cast<float>(m_CurrentGauge.MaxHP));
}
/// <summary>
/// MP 비율 반환
/// </summary>
/// <returns>음수면 실패</returns>
const float UMyStatComponent::GetMPRatio()
{
	if (m_CurrentGauge.MaxMP == 0)
		return -1.f;
	return (static_cast<float>(m_CurrentGauge.MP) / static_cast<float>(m_CurrentGauge.MaxMP));
}
/// <summary>
/// SP 비율 반환
/// </summary>
/// <returns>음수면 실패</returns>
const float UMyStatComponent::GetSPRatio()
{
	if (m_CurrentGauge.MaxSP == 0)
		return -1.f;
	return (static_cast<float>(m_CurrentGauge.SP) / static_cast<float>(m_CurrentGauge.MaxSP));
}

void UMyStatComponent::SetHP(int32 HP)
{
	m_CurrentGauge.HP = FMath::Clamp(HP, 0, m_CurrentGauge.MaxHP);

	OnHPChanged.Broadcast(GetHPRatio());
	if (m_CurrentGauge.HP == 0)
	{
		OnDeathCheck.Broadcast();
	}

	if (m_CurrentGauge.HP == m_CurrentGauge.MaxHP)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(m_Timer_HPRecovery) == true)
			GetWorld()->GetTimerManager().ClearTimer(m_Timer_HPRecovery);
	}

}

void UMyStatComponent::SetMP(int32 MP)
{
	m_CurrentGauge.MP = FMath::Clamp(MP, 0, m_CurrentGauge.MaxMP);

	OnMPChanged.Broadcast(GetMPRatio());
	if (m_CurrentGauge.MP == m_CurrentGauge.MaxMP)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(m_Timer_MPRecovery) == true)
			GetWorld()->GetTimerManager().ClearTimer(m_Timer_MPRecovery);
	}

}

void UMyStatComponent::SetSP(int32 SP)
{
	m_CurrentGauge.SP = FMath::Clamp(SP, 0, m_CurrentGauge.MaxSP);

	OnSPChanged.Broadcast(GetSPRatio());
	if (m_CurrentGauge.SP == m_CurrentGauge.MaxSP)
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(m_Timer_SPRecovery)==true) 
			GetWorld()->GetTimerManager().ClearTimer(m_Timer_SPRecovery);
	}

}

void UMyStatComponent::AddHP(int32 HP)
{
	if (HP > 0)
	{
		// 증가 
	}
	else if (HP < 0)
	{
		// 감소
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_HPRecovery);
		GetWorld()->GetTimerManager().SetTimer(m_Timer_HPRecovery,this,&UMyStatComponent::Recovery_HP,5.f,false);
	}
	else
	{
		// 변화없으면 아무일도 없도록 한다.
		UE_LOG(LogTemp, Warning, TEXT("Noting HP"));
		return;
	}
	SetHP(m_CurrentGauge.HP + HP);
}

void UMyStatComponent::AddMP(int32 MP)
{
	if (MP > 0)
	{
		// 증가 
	}
	else if (MP < 0)
	{
		// 감소
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_MPRecovery);
		GetWorld()->GetTimerManager().SetTimer(m_Timer_MPRecovery, this, &UMyStatComponent::Recovery_MP, 5.f, false);

	}
	else
	{
		// 변화없으면 아무일도 없도록 한다.
		UE_LOG(LogTemp, Warning, TEXT("Noting MP"));
		return;
	}
	SetMP(m_CurrentGauge.MP + MP);
}

void UMyStatComponent::AddSP(int32 SP)
{
	if (SP > 0)
	{
		// 증가 
	}
	else if (SP < 0)
	{
		// 감소
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_SPRecovery);
		GetWorld()->GetTimerManager().SetTimer(m_Timer_SPRecovery, this, &UMyStatComponent::Recovery_SP, 5.f, false);

	}
	else
	{
		// 변화없으면 아무일도 없도록 한다.
		UE_LOG(LogTemp, Warning, TEXT("Noting SP"));
		return;
	}
	SetSP(m_CurrentGauge.SP + SP);
}
#pragma endregion

bool UMyStatComponent::CanPay(TMap<EGauge, int32>& Costs)
{
	for (auto& Cost : Costs)
	{
		switch (Cost.Key)
		{
			case EGauge::HP:
			{
				if (m_CurrentGauge.HP < Cost.Value)
					return false;
				break;

			}
			case EGauge::MP:
			{
				if (m_CurrentGauge.MP < Cost.Value)
					return false;
				break;
			}
			case EGauge::SP:
			{
				if (m_CurrentGauge.SP < Cost.Value)
					return false;
				break;
			}
			default:
				break;
		}
	}

	return true;
}

void UMyStatComponent::Recovery_HP()
{
	GetWorld()->GetTimerManager().SetTimer(m_Timer_HPRecovery, this, &UMyStatComponent::Recovery_HP, 1.f, true);
	
	// (임시) 비율을 100분의 1로 하드코딩해두었다. 최소 값도 1로 임시 보정
	AddHP(FMath::Clamp((GetTotalStat().HP / 100),1, GetTotalStat().HP));
	UE_LOG(LogTemp, Warning, TEXT("Recovery_HP %d"), m_CurrentGauge.HP);
}

void UMyStatComponent::Recovery_MP()
{
	GetWorld()->GetTimerManager().SetTimer(m_Timer_MPRecovery, this, &UMyStatComponent::Recovery_MP, 1.f, true);

	// (임시) 비율을 100분의 1로 하드코딩해두었다. 최소 값도 1로 임시 보정
	AddMP(FMath::Clamp((GetTotalStat().MP / 100), 1, GetTotalStat().MP));
	UE_LOG(LogTemp, Warning, TEXT("Recovery_MP %d"), m_CurrentGauge.MP);
}

void UMyStatComponent::Recovery_SP()
{
	GetWorld()->GetTimerManager().SetTimer(m_Timer_SPRecovery, this, &UMyStatComponent::Recovery_SP, 1.f, true);

	// (임시) 비율을 100분의 1로 하드코딩해두었다. 최소 값도 1로 임시 보정
	AddSP(FMath::Clamp((GetTotalStat().SP / 100), 1, GetTotalStat().SP));
	UE_LOG(LogTemp, Warning, TEXT("Recovery_SP %d"), m_CurrentGauge.SP);
}

