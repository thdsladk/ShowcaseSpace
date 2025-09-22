// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "MyStatComponent.h"
#include "AbilityComponent.h"
#include "MyTest_TopDownPlayerController.h"
#include "SlotWidget.h"
#include "Data/MyGameSubsystem.h"


constexpr float FullPercent = 1.0f;
constexpr float EmptyPercent = 0.0f;
constexpr float CoolDownTimerRate = 0.01f;

void UMyHUD::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(m_StatSync);
}

bool UMyHUD::Initialize()
{
	bool Result = Super::Initialize();

	Btn_Item1->Initialize();
	Btn_Item2->Initialize();
	Btn_Item3->Initialize();
	Btn_Item4->Initialize();
	Btn_Item5->Initialize();
	Btn_Item6->Initialize();

	GetWorld()->GetTimerManager().SetTimer(m_StatSync, this, &UMyHUD::Update_StatRatioSync, 0.1f, true);
	m_TimerSkillCoolDown.Add(ESkill::Skill_Q, FTimerHandle());
	m_TimerSkillCoolDown.Add(ESkill::Skill_W, FTimerHandle());
	m_TimerSkillCoolDown.Add(ESkill::Skill_E, FTimerHandle());
	m_TimerSkillCoolDown.Add(ESkill::Skill_R, FTimerHandle());

	// (임시) 하드 코딩으로 들어가 있다 어빌리티들이... 
	//m_SkillTag.Add(ESkill::Skill_Q, FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Q")));
	m_SkillTag.Add(ESkill::Skill_W, FGameplayTag::RequestGameplayTag(FName("Cooldown.Skill.StormKill")));
	m_SkillTag.Add(ESkill::Skill_E, FGameplayTag::RequestGameplayTag(FName("Cooldown.Skill.FireBall")));
	m_SkillTag.Add(ESkill::Skill_R, FGameplayTag::RequestGameplayTag(FName("Cooldown.Skill.WaterBall")));

	return Result;
}

void UMyHUD::BindStatus(UMyStatComponent* StatComp)
{
	StatComp->OnHPChanged.AddUObject(this, &UMyHUD::UpdateHP);
	StatComp->OnMPChanged.AddUObject(this, &UMyHUD::UpdateMP);
	StatComp->OnSPChanged.AddUObject(this, &UMyHUD::UpdateSP);

	m_PreStatRatio.HP = StatComp->GetHPRatio();

	// Screen1 Bind 
	StatComp->OnHPChanged.AddUObject(this, &UMyHUD::UpDateScreen1);


	// 첫 초기화 이후 업데이트.
	UpdateHP(StatComp->GetHPRatio());
	UpdateMP(StatComp->GetMPRatio());
	UpdateSP(StatComp->GetSPRatio());
}

void UMyHUD::UpdateHP(float Hp)
{
	if (m_StatSync.IsValid())
	{
		m_CurrentStatRatio.HP = Hp;
		ProgressBar_HP->SetPercent(m_CurrentStatRatio.HP);
	}
	// Debug
	UE_LOG(LogTemp, Log, TEXT(" Current Hp == %f"), m_CurrentStatRatio.HP);
}

void UMyHUD::UpdateMP(float Mp)
{
	if (m_StatSync.IsValid())
	{
		m_CurrentStatRatio.MP = Mp;
		ProgressBar_MP->SetPercent(m_CurrentStatRatio.MP);
	}
	// Debug
	UE_LOG(LogTemp, Log, TEXT(" Current Mp == %f"), m_CurrentStatRatio.MP);
}

void UMyHUD::UpdateSP(float Sp)
{
	if (m_StatSync.IsValid())
	{
		m_CurrentStatRatio.SP = Sp;
		ProgressBar_SP->SetPercent(m_CurrentStatRatio.SP);
	}
	// Debug
	UE_LOG(LogTemp, Log, TEXT(" Current Sp == %f"), m_CurrentStatRatio.SP);

}

void UMyHUD::Update_StatRatioSync()
{
	// HP
	if (FMath::IsNearlyEqual(m_CurrentStatRatio.HP, m_PreStatRatio.HP) == false)
	{
		m_PreStatRatio.HP = FMath::Clamp<float>(m_PreStatRatio.HP + (m_CurrentStatRatio.HP - m_PreStatRatio.HP) * 0.2, m_CurrentStatRatio.HP, m_PreStatRatio.HP);
		ProgressBar_BackGroundHP->SetPercent(m_PreStatRatio.HP);
	}
	else
	{
		// Debug
		//UE_LOG(LogTemp,Log,TEXT(" Sync Current Hp == Pre Hp "))
	}
	// MP
	if (FMath::IsNearlyEqual(m_CurrentStatRatio.MP, m_PreStatRatio.MP) == false)
	{
		m_PreStatRatio.MP = FMath::Clamp<float>(m_PreStatRatio.MP + (m_CurrentStatRatio.MP - m_PreStatRatio.MP) * 0.2, m_CurrentStatRatio.MP, m_PreStatRatio.MP);
		ProgressBar_BackGroundMP->SetPercent(m_PreStatRatio.MP);
	}
	// SP
	if (FMath::IsNearlyEqual(m_CurrentStatRatio.SP, m_PreStatRatio.SP) == false)
	{
		m_PreStatRatio.SP = FMath::Clamp<float>(m_PreStatRatio.SP + (m_CurrentStatRatio.SP - m_PreStatRatio.SP) * 0.2, m_CurrentStatRatio.SP, m_PreStatRatio.SP);
		ProgressBar_BackGroundSP->SetPercent(m_PreStatRatio.SP);
	}
}

void UMyHUD::BindAbility(UAbilityComponent* AbilityComp)
{
	if (AbilityComp != nullptr)
	{
		m_pAbilityComp = AbilityComp;
	}

	Btn_Skill1->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtn_Skill1);
	Btn_Skill2->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtn_Skill2);
	Btn_Skill3->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtn_Skill3);
	Btn_Skill4->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtn_Skill4);

	Btn_Skill1->OnReleased.AddDynamic(this, &UMyHUD::ReleaseBtnSkill1);
	Btn_Skill2->OnReleased.AddDynamic(this, &UMyHUD::ReleaseBtnSkill2);
	Btn_Skill3->OnReleased.AddDynamic(this, &UMyHUD::ReleaseBtnSkill3);
	Btn_Skill4->OnReleased.AddDynamic(this, &UMyHUD::ReleaseBtnSkill4);


	Btn_OpenChest->OnReleased.AddDynamic(this, &UMyHUD::Click_Tab);


	UMyGameSubsystem* SubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();

	if (SubSystem != nullptr)
	{
		m_SkillUIData.Init(FSkillUIData{}, static_cast<int32>(ESkill::End));
		for (int32 i = 0; i < static_cast<int32>(ESkill::End); ++i)
		{
			const FSkillData* SkillData = SubSystem->GetSkillData(i);
			m_SkillUIData[i].Icon = SkillData->Icon;
			m_SkillUIData[i].CoolDown = SkillData->CoolDown;
			m_SkillUIData[i].ChargeCount = 0;
			m_SkillUIData[i].UIState = static_cast<uint8>(ESKillUIState::Enable);
	
		}
	
		// (임시) 스킬을 테이블정보로 세팅하는것에 대하여 고민중... 버튼이라서...
		const FSkillData* SkillData = SubSystem->GetSkillData(static_cast<int32>(ESkill::Skill_Q));
	
		//FButtonStyle Style = Btn_Skill1->WidgetStyle;
		FSlateBrush Brush;
		Brush.SetResourceObject(SkillData->Icon);
		Btn_Skill1->WidgetStyle.SetNormal(Brush);
		Btn_Skill1->WidgetStyle.SetHovered(Brush);
		Btn_Skill1->WidgetStyle.SetPressed(Brush);
		Btn_Skill1->WidgetStyle.SetDisabled(Brush);
		//Btn_Skill1->SetStyle(Style);
	
	}
}

void UMyHUD::UpdateBtn_Skill1()
{
	m_OnSkill.Broadcast(static_cast<uint8>(ESkill::Skill_Q));
}

void UMyHUD::UpdateBtn_Skill2()
{
	m_OnSkill.Broadcast(static_cast<uint8>(ESkill::Skill_W));
}

void UMyHUD::UpdateBtn_Skill3()
{
	m_OnSkill.Broadcast(static_cast<uint8>(ESkill::Skill_E));
}

void UMyHUD::UpdateBtn_Skill4()
{
	m_OnSkill.Broadcast(static_cast<uint8>(ESkill::Skill_R));
}

void UMyHUD::ReleaseBtnSkill1()
{
	// 여기서 쿨다운 처리.
	if (BtnSkillCoolDown1 != nullptr)
	{
		PlayAnimation(BtnSkillCoolDown1, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		
		if (GetWorld()->GetTimerManager().IsTimerActive(m_TimerSkillCoolDown[ESkill::Skill_Q]) == false)
		{
			ProgressBar_Skill1->SetPercent(FullPercent);
			// CoolDown Start
			GetWorld()->GetTimerManager().SetTimer(m_TimerSkillCoolDown[ESkill::Skill_Q],this, &UMyHUD::TimerBtnSkill1, CoolDownTimerRate, true, CoolDownTimerRate);
		}
		
	}

}

void UMyHUD::ReleaseBtnSkill2()
{
	// 여기서 쿨다운 처리.
	if (BtnSkillCoolDown2 != nullptr)
	{
		PlayAnimation(BtnSkillCoolDown2, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);

		if (GetWorld()->GetTimerManager().IsTimerActive(m_TimerSkillCoolDown[ESkill::Skill_W]) == false)
		{
			ProgressBar_Skill2->SetPercent(FullPercent);
			// CoolDown Start
			GetWorld()->GetTimerManager().SetTimer(m_TimerSkillCoolDown[ESkill::Skill_W], this, &UMyHUD::TimerBtnSkill2, CoolDownTimerRate, true, CoolDownTimerRate);
		}

	}
}

void UMyHUD::ReleaseBtnSkill3()
{
	// 여기서 쿨다운 처리.
	if (BtnSkillCoolDown3 != nullptr)
	{
		PlayAnimation(BtnSkillCoolDown3, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);

		if (GetWorld()->GetTimerManager().IsTimerActive(m_TimerSkillCoolDown[ESkill::Skill_E]) == false)
		{
			ProgressBar_Skill3->SetPercent(FullPercent);
			// CoolDown Start
			GetWorld()->GetTimerManager().SetTimer(m_TimerSkillCoolDown[ESkill::Skill_E], this, &UMyHUD::TimerBtnSkill3, CoolDownTimerRate, true, CoolDownTimerRate);
		}

	}
}

void UMyHUD::ReleaseBtnSkill4()
{
	// 여기서 쿨다운 처리.
	if (BtnSkillCoolDown4 != nullptr)
	{
		PlayAnimation(BtnSkillCoolDown4, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);

		if (GetWorld()->GetTimerManager().IsTimerActive(m_TimerSkillCoolDown[ESkill::Skill_R]) == false)
		{
			ProgressBar_Skill4->SetPercent(FullPercent);
			// CoolDown Start
			GetWorld()->GetTimerManager().SetTimer(m_TimerSkillCoolDown[ESkill::Skill_R], this, &UMyHUD::TimerBtnSkill4, CoolDownTimerRate, true, CoolDownTimerRate);
		}

	}
}

void UMyHUD::ReleaseBtn(uint8 Btn)
{
	switch (static_cast<ESkill>(Btn))
	{
	case ESkill::Skill_Q:
	{
		ReleaseBtnSkill1();
		break;
	}
	case ESkill::Skill_W:
	{
		ReleaseBtnSkill2();
		break;
	}
	case ESkill::Skill_E:
	{
		ReleaseBtnSkill3();
		break;
	}
	case ESkill::Skill_R:
	{
		ReleaseBtnSkill4();
		break;
	}

	default:
		break;
	}
}

void UMyHUD::TimerBtnSkill1()
{
	// 0~1 사이의 값이 온다.
	double Time = m_pAbilityComp->GetCooldownRatio(m_SkillTag[ESkill::Skill_Q]);


	if (Time >= 1.0)
	{
		ProgressBar_Skill1->SetPercent(1.0 - Time);

		GetWorld()->GetTimerManager().ClearTimer(m_TimerSkillCoolDown[(ESkill::Skill_Q)]);
		StopAnimation(BtnSkillCoolDown1);
	}
	else
	{
		ProgressBar_Skill1->SetPercent(1.0 - Time);
	}
}

void UMyHUD::TimerBtnSkill2()
{
	// 0~1 사이의 값이 온다.
	double Time = m_pAbilityComp->GetCooldownRatio(m_SkillTag[ESkill::Skill_W]);


	if (Time >= 1.0)
	{
		ProgressBar_Skill2->SetPercent(1.0 - Time);

		GetWorld()->GetTimerManager().ClearTimer(m_TimerSkillCoolDown[(ESkill::Skill_W)]);
		StopAnimation(BtnSkillCoolDown2);
	}
	else
	{
		ProgressBar_Skill2->SetPercent(1.0 - Time);
	}
}

void UMyHUD::TimerBtnSkill3()
{
	// 0~1 사이의 값이 온다.
	double Time = m_pAbilityComp->GetCooldownRatio(m_SkillTag[ESkill::Skill_E]);


	if (Time >= 1.0)
	{
		ProgressBar_Skill3->SetPercent(1.0 - Time);

		GetWorld()->GetTimerManager().ClearTimer(m_TimerSkillCoolDown[(ESkill::Skill_E)]);
		StopAnimation(BtnSkillCoolDown3);
	}
	else
	{
		ProgressBar_Skill3->SetPercent(1.0 - Time);
	}
}

void UMyHUD::TimerBtnSkill4()
{
	// 0~1 사이의 값이 온다.
	double Time = m_pAbilityComp->GetCooldownRatio(m_SkillTag[ESkill::Skill_R]);


	if (Time >= 1.0)
	{
		ProgressBar_Skill4->SetPercent(1.0 - Time);

		GetWorld()->GetTimerManager().ClearTimer(m_TimerSkillCoolDown[(ESkill::Skill_R)]);
		StopAnimation(BtnSkillCoolDown4);
	}
	else
	{
		ProgressBar_Skill4->SetPercent(1.0 - Time);
	}
}

void UMyHUD::Click_Tab()
{
	//CastChecked<AMyTest_TopDownPlayerController>(GetWorld()->GetFirstPlayerController())->Inventory_Notify.Broadcast();
	m_OnInven.Broadcast();
}

void UMyHUD::BindScreen1()
{
	Img_Screen1->SetVisibility(ESlateVisibility::Hidden);

	// (임시) 일단 임시로 깜빡임 효과를 꺼두자.
	//GetWorld()->GetTimerManager().SetTimer(m_Timer_Screen1, this, &UMyHUD::UpDateScreen1, 1.f, true, 0.f);

}

void UMyHUD::UpDateScreen1(float Hp)
{
	if (
		Hp < 0.5f &&
		Img_Screen1->GetVisibility()== ESlateVisibility::Hidden)
	{
		
		Img_Screen1->SetVisibility(ESlateVisibility::Visible);

	}
	else if(
		Hp >= 0.5f &&
		Img_Screen1->GetVisibility() == ESlateVisibility::Visible)
	{
		Img_Screen1->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FLinearColor LinearColor{ 0.f,0.f,0.f,0.05f };
	if (Img_Screen1->GetColorAndOpacity().A >= 0.05f)
	{
		Img_Screen1->SetColorAndOpacity(Img_Screen1->GetColorAndOpacity() - LinearColor);

	}
	else
	{
		LinearColor.A = 0.2f;
		Img_Screen1->SetColorAndOpacity(Img_Screen1->GetColorAndOpacity() + LinearColor);

	}


	
}


