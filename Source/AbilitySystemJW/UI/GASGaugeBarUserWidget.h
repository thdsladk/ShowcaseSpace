// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "GASGaugeBarUserWidget.generated.h"

class UOverlay;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASGaugeBarUserWidget : public UGASUserWidget
{
	GENERATED_BODY()
public:
	UGASGaugeBarUserWidget();
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	virtual void OnHPChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHPChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMPChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxMPChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnSPChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxSPChanged(const FOnAttributeChangeData& ChangeData);

	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHpBar();
	void UpdateMpBar();
	void UpdateSpBar();

public:
	UFUNCTION(BlueprintCallable)
	void SetVisibleHPBar(bool bVisible) { m_VisibleHP = bVisible; }
	UFUNCTION(BlueprintCallable)
	void SetVisibleMPBar(bool bVisible) { m_VisibleMP = bVisible; }
	UFUNCTION(BlueprintCallable)
	void SetVisibleSPBar(bool bVisible) { m_VisibleSP = bVisible; }



protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OverlayHP;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OverlayMP;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OverlaySP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> MpBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MpText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> SpBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SpText;


	float m_CurrentHP = 0.0f;
	float m_CurrentMaxHP = 0.0f;
	float m_CurrentMP = 0.0f;
	float m_CurrentMaxMP = 0.0f;
	float m_CurrentSP = 0.0f;
	float m_CurrentMaxSP = 0.0f;

	FLinearColor m_HPColor = FLinearColor::Red;
	FLinearColor m_MPColor = FLinearColor::Blue;
	FLinearColor m_SPColor = FLinearColor::Yellow;

	FLinearColor m_InvinsibleColor = FLinearColor(0.4f,0.0f,0.6f,1.0f);

	uint8 m_VisibleHP :1;
	uint8 m_VisibleMP :1;
	uint8 m_VisibleSP :1;

};
