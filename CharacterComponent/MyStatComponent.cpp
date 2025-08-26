// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/MyStatComponent.h"
#include "Data/MyGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

/*
	HP,MP,SP ���� ���������� 
	Add �Լ��� ������ ������ ( ���ο� Set�Լ��� ���ؼ� ��ȭ��Ų��. ) 
	Set �Լ��� ���� ���� ���Ҷ� ��� 

	���� ������ ��ȭ�� Add�Լ��� ���ؼ� �ϸ�ȴ�. 
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

	/* �̽����� �ʱ�ȭ ���־�� PostInitializeComponents���� ���� ȣ��Ǿ ������ ����.
		������ ����� �ʱ�ȭ �Ǳ� ���� ȣ���ϸ� ���� �߸� ȣ��Ǵ� ������ �־��� .
		HP�� MaxHp�� 0.0�� �Ǿ 0.0/0.0�� �ع����� ���� �����. ( Log... ) 
		!!! ������� �и� 0�� �Ǵ� ��츦 �������� !!! ���� �������� ������ ������.. 
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
// ������ ���� ������ ���� ������ �Ǿ���. ������ ���� ���� ��ȭ�� ������ �ý������� �����ϵ��� �� ����
// (�ӽ�) �� �Լ��� ���� �Ǿ�� �Ѵ�. 
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
	// (�ӽ�) �ִ밪�� 10.f�� ���Ƿ� �ߴ�.
	m_BaseStat.AttackSpeed = FMath::Clamp(Rate, 0.f, 10.f);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetModifierAttackSpeed(float Rate)
{
	// (�ӽ�) �ִ밪�� 10.f�� ���Ƿ� �ߴ�.
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
/// HP ���� ��ȯ
/// </summary>
/// <returns>������ ����</returns>
const float UMyStatComponent::GetHPRatio()
{ 
	if (m_CurrentGauge.MaxHP == 0)
		return -1.f;
	return (static_cast<float>(m_CurrentGauge.HP) / static_cast<float>(m_CurrentGauge.MaxHP));
}
/// <summary>
/// MP ���� ��ȯ
/// </summary>
/// <returns>������ ����</returns>
const float UMyStatComponent::GetMPRatio()
{
	if (m_CurrentGauge.MaxMP == 0)
		return -1.f;
	return (static_cast<float>(m_CurrentGauge.MP) / static_cast<float>(m_CurrentGauge.MaxMP));
}
/// <summary>
/// SP ���� ��ȯ
/// </summary>
/// <returns>������ ����</returns>
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
		// ���� 
	}
	else if (HP < 0)
	{
		// ����
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_HPRecovery);
		GetWorld()->GetTimerManager().SetTimer(m_Timer_HPRecovery,this,&UMyStatComponent::Recovery_HP,5.f,false);
	}
	else
	{
		// ��ȭ������ �ƹ��ϵ� ������ �Ѵ�.
		UE_LOG(LogTemp, Warning, TEXT("Noting HP"));
		return;
	}
	SetHP(m_CurrentGauge.HP + HP);
}

void UMyStatComponent::AddMP(int32 MP)
{
	if (MP > 0)
	{
		// ���� 
	}
	else if (MP < 0)
	{
		// ����
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_MPRecovery);
		GetWorld()->GetTimerManager().SetTimer(m_Timer_MPRecovery, this, &UMyStatComponent::Recovery_MP, 5.f, false);

	}
	else
	{
		// ��ȭ������ �ƹ��ϵ� ������ �Ѵ�.
		UE_LOG(LogTemp, Warning, TEXT("Noting MP"));
		return;
	}
	SetMP(m_CurrentGauge.MP + MP);
}

void UMyStatComponent::AddSP(int32 SP)
{
	if (SP > 0)
	{
		// ���� 
	}
	else if (SP < 0)
	{
		// ����
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_SPRecovery);
		GetWorld()->GetTimerManager().SetTimer(m_Timer_SPRecovery, this, &UMyStatComponent::Recovery_SP, 5.f, false);

	}
	else
	{
		// ��ȭ������ �ƹ��ϵ� ������ �Ѵ�.
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
	
	// (�ӽ�) ������ 100���� 1�� �ϵ��ڵ��صξ���. �ּ� ���� 1�� �ӽ� ����
	AddHP(FMath::Clamp((GetTotalStat().HP / 100),1, GetTotalStat().HP));
	UE_LOG(LogTemp, Warning, TEXT("Recovery_HP %d"), m_CurrentGauge.HP);
}

void UMyStatComponent::Recovery_MP()
{
	GetWorld()->GetTimerManager().SetTimer(m_Timer_MPRecovery, this, &UMyStatComponent::Recovery_MP, 1.f, true);

	// (�ӽ�) ������ 100���� 1�� �ϵ��ڵ��صξ���. �ּ� ���� 1�� �ӽ� ����
	AddMP(FMath::Clamp((GetTotalStat().MP / 100), 1, GetTotalStat().MP));
	UE_LOG(LogTemp, Warning, TEXT("Recovery_MP %d"), m_CurrentGauge.MP);
}

void UMyStatComponent::Recovery_SP()
{
	GetWorld()->GetTimerManager().SetTimer(m_Timer_SPRecovery, this, &UMyStatComponent::Recovery_SP, 1.f, true);

	// (�ӽ�) ������ 100���� 1�� �ϵ��ڵ��صξ���. �ּ� ���� 1�� �ӽ� ����
	AddSP(FMath::Clamp((GetTotalStat().SP / 100), 1, GetTotalStat().SP));
	UE_LOG(LogTemp, Warning, TEXT("Recovery_SP %d"), m_CurrentGauge.SP);
}

