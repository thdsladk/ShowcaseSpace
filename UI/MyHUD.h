// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Header/DataStruct.h"
#include "MyHUD.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill, uint8);
DECLARE_MULTICAST_DELEGATE(FOnInventory);

class UProgressBar;
class UTextBlock;
class UButton;
class USlotWidget;

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct()override;
	virtual bool Initialize() override;

public:

	void BindStatus(class UMyStatComponent* StatComp);
	void BindSkill(class USkillComponent* SkillComp);
	
	void UpdateHP(float Hp);
	void UpdateMP(float Mp);
	void UpdateSP(float Sp);

	void Update_StatRatioSync();


	UFUNCTION()
	void UpdateBtnQ();
	UFUNCTION()
	void UpdateBtnW();
	UFUNCTION()
	void UpdateBtnE();
	UFUNCTION()
	void UpdateBtnR();

	UFUNCTION()
	void ReleaseBtnQ();
	UFUNCTION()
	void ReleaseBtnW();
	UFUNCTION()
	void ReleaseBtnE();
	UFUNCTION()
	void ReleaseBtnR();
	UFUNCTION()
	void ReleaseBtn(uint8 Btn);

	void TimerBtnQ();
	void TimerBtnW();
	void TimerBtnE();
	void TimerBtnR();



	UFUNCTION()
	void Click_Tab();

	void BindScreen1();
	void UpDateScreen1(float Hp);

	FOnSkill m_OnSkill;
	FOnInventory m_OnInven;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_HP;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_HP;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_BackGroundHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_MP;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_MP;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_BackGroundMP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SP;
	

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_SP;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_BackGroundSP;


	// Button Widget Skill

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Skill1;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Skill1;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Skill2;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Skill2;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Skill3;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Skill3;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Skill4;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Skill4;


	// Button Widget Main/Sub

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Main;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Sub;

public:
	// Button Widget Inventory

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_OpenChest;

private:
	// Button Widget Item

	UPROPERTY(meta = (BindWidget))
	USlotWidget* Btn_Item1;
	UPROPERTY(meta = (BindWidget))
	USlotWidget* Btn_Item2;
	UPROPERTY(meta = (BindWidget))
	USlotWidget* Btn_Item3;
	UPROPERTY(meta = (BindWidget))
	USlotWidget* Btn_Item4;
	UPROPERTY(meta = (BindWidget))
	USlotWidget* Btn_Item5;
	UPROPERTY(meta = (BindWidget))
	USlotWidget* Btn_Item6;


	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Screen1;
	
	// Animation
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BtnSkillCoolDown1;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BtnSkillCoolDown2;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BtnSkillCoolDown3;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BtnSkillCoolDown4;


	// Timer 
	FTimerHandle m_Timer_Screen1;
	FTimerHandle m_StatSync;
	TArray<FTimerHandle> m_TimerSkillCoolDown;

private:		
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//double m_PreHpRatio = 1.0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//double m_PreMpRatio = 1.0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//double m_PreSpRatio = 1.0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//double m_CurrentHpRatio = 1.0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//double m_CurrentMpRatio = 1.0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//double m_CurrentSpRatio = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FStatVector m_PreStatRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FStatVector m_CurrentStatRatio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FSkillUIData> m_SkillUIData;


	
};
