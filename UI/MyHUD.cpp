// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "MyStatComponent.h"
#include "SkillComponent.h"
#include "MyTest_TopDownPlayerController.h"
#include "SlotWidget.h"

void UMyHUD::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(m_HpSync);
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

	GetWorld()->GetTimerManager().SetTimer(m_HpSync, this, &UMyHUD::Update_HpRatioSync, 0.1f, true);


	return Result;
}

void UMyHUD::BindHP(UMyStatComponent* StatComp)
{
	m_StatComp = StatComp;
	StatComp->OnHPChanged.AddUObject(this, &UMyHUD::UpdateHP);
	StatComp->OnMPChanged.AddUObject(this, &UMyHUD::UpdateMP);
	StatComp->OnSPChanged.AddUObject(this, &UMyHUD::UpdateSP);

	m_PreHpRatio = m_StatComp->GetHPRatio();

	// Screen1 Bind 
	StatComp->OnHPChanged.AddUObject(this, &UMyHUD::UpDateScreen1);



}

void UMyHUD::UpdateHP()
{
	if (m_StatComp.IsValid())
	{
		if (m_HpSync.IsValid())
		{
			m_CurrentHpRatio = m_StatComp->GetHPRatio();
			
		}
		// Debug
		UE_LOG(LogTemp, Log, TEXT(" Current Hp == %f"), m_CurrentHpRatio);
		
	}
}

void UMyHUD::UpdateMP()
{
	if (m_StatComp.IsValid())
	{
		m_CurrentMpRatio = m_StatComp->GetMPRatio();
		ProgressBar_MP->SetPercent(m_CurrentMpRatio);
	}
}

void UMyHUD::UpdateSP()
{
	if (m_StatComp.IsValid())
	{
		m_CurrentSpRatio = m_StatComp->GetSPRatio();
		ProgressBar_SP->SetPercent(m_CurrentSpRatio);
	}
}

void UMyHUD::Update_HpRatioSync()
{
	if (FMath::IsNearlyEqual(m_CurrentHpRatio, m_PreHpRatio) == false)
	{
		m_PreHpRatio = FMath::Clamp<float>(m_PreHpRatio + (m_CurrentHpRatio - m_PreHpRatio) * 0.2, m_CurrentHpRatio, m_PreHpRatio);
		ProgressBar_HP->SetPercent(m_PreHpRatio);
	}
	else
	{
		// Debug
		//UE_LOG(LogTemp,Log,TEXT(" Sync Current Hp == Pre Hp "))
	}
}

void UMyHUD::BindSkill(USkillComponent* SkillComp)
{
	Btn_Skill1->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnQ);
	Btn_Skill2->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnW);
	Btn_Skill3->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnE);
	Btn_Skill4->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnR);

	Btn_OpenChest->OnClicked.AddDynamic(this, &UMyHUD::Click_Tab);

	m_OnSkill.AddUObject(SkillComp, &USkillComponent::Click_Btn);

}

void UMyHUD::UpdateBtnQ()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_Q);
}

void UMyHUD::UpdateBtnW()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_W);
}

void UMyHUD::UpdateBtnE()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_E);
}

void UMyHUD::UpdateBtnR()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_R);
}

void UMyHUD::Click_Tab()
{
	//CastChecked<AMyTest_TopDownPlayerController>(GetWorld()->GetFirstPlayerController())->Inventory_Notify.Broadcast();
	m_OnInven.Broadcast();
}

void UMyHUD::BindScreen1()
{
	Img_Screen1->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().SetTimer(m_Timer_Screen1, this, &UMyHUD::UpDateScreen1, 1.f, true, 0.f);

}

void UMyHUD::UpDateScreen1()
{
	if (m_StatComp.IsValid())
	{
		float RatioHP = m_StatComp.Get()->GetHPRatio();

		if (
			RatioHP < 0.5f && 
			Img_Screen1->GetVisibility()== ESlateVisibility::Hidden)
		{
			
			Img_Screen1->SetVisibility(ESlateVisibility::Visible);

		}
		else if(
			RatioHP >= 0.5f && 
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

}


