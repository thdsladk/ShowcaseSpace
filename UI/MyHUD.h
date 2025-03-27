// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill, uint8);
DECLARE_MULTICAST_DELEGATE(FOnInventory);

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

	void BindHP(class UMyStatComponent* StatComp);
	void BindSkill(class USkillComponent* SkillComp);
	
	void UpdateHP();
	void UpdateMP();
	void UpdateSP();

	void Update_HpRatioSync();


	UFUNCTION()
	void UpdateBtnQ();
	UFUNCTION()
	void UpdateBtnW();
	UFUNCTION()
	void UpdateBtnE();
	UFUNCTION()
	void UpdateBtnR();

	UFUNCTION()
	void Click_Tab();

	void BindScreen1();
	void UpDateScreen1();

	FOnSkill m_OnSkill;
	FOnInventory m_OnInven;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_HP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_MP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_SP;


	// Button Widget Skill

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill1;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill2;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill3;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill4;


	// Button Widget Main/Sub

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Main;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Sub;

public:
	// Button Widget Inventory

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	class UButton* Btn_OpenChest;

private:
	// Button Widget Item

	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item1;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item2;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item3;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item4;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item5;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item6;


	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Screen1;


	TWeakObjectPtr<UMyStatComponent> m_StatComp;

	// Timer 
	FTimerHandle m_Timer_Screen1;
	FTimerHandle m_HpSync;

private:		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double m_PreHpRatio = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double m_PreMpRatio = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double m_PreSpRatio = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double m_CurrentHpRatio = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double m_CurrentMpRatio = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double m_CurrentSpRatio = 1.0;


	
};
