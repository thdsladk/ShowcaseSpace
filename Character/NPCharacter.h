// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/MyMonsterWidgetInterface.h"
#include "Interface/HighlightInterface.h"
#include "NPCharacter.generated.h"


class AMyEquipment;
class UWidgetComponent;
class UMyUserWidget;
class UUserWidget;
class UInputComponent;
class AController;

/*
	클래스 개요


*/
UCLASS(config=NPCList)
class MYTEST_TOPDOWN_API ANPCharacter : public ACharacterBase, public IMyMonsterWidgetInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ANPCharacter(); 

protected:
	virtual void BeginPlay() override;
	virtual void SetupCharacterData()override;
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

#pragma region Behavior Function
	// Behavior
	virtual const uint8 GetAggroGauge()override { return m_AggroGauge; };
	virtual void SetAggroGauge(const uint8 AggroGauge)override;
	virtual const float GetAggroGauge_Ratio()override { return static_cast<float>(m_AggroGauge) / 100.f; };

	virtual void OnIdle()override;
	virtual void OnBattle()override;
	//virtual void AttackCheck()override;
	virtual void Death()override;
	virtual void DeathEnd()override;
	virtual void OnDefense()override;
	virtual void StopDefense()override;
	virtual void Defense_Hit()override;
	virtual void OnDetect()override;
	virtual void DetectEnd()override;
	virtual void OnAlert()override;

	virtual void OnCommand(uint8 Command)override;

	virtual bool IsOnTriggerEscape()override;
#pragma endregion

	// Highlight Section
public:
	virtual void HighlightActor()override;
	virtual void UnHighlightActor()override;


#pragma region Animation Function
public:
	// Animation Function			// 몽타주의 형태를 정형화 해버리면 종속될 위험도 생각하자.
	UFUNCTION()
	void OnBehaviorMontageEnded();
#pragma endregion

	// Damage Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual bool IsPlayerCharacter() override{ return false; }

	// Render Section
	virtual void ApplyHidden(const bool bHide);
protected:
	// Widget Section
	virtual void SetupMonsterWidget(UMyUserWidget* InUserWidget) override;

#pragma region Components Member
protected:
	UPROPERTY(VisibleAnywhere, Category = "WidgetComp")
	TObjectPtr<UWidgetComponent> m_GaugeBar;
	UPROPERTY(VisibleAnywhere, Category = "WidgetComp")
	TObjectPtr<UWidgetComponent>m_Emotion;

	// 클래스만 먼저 정의하고 생성해서 쓰는 방식 
	// WidgetComponent로 붙이는 방식과 다르게 사용  
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> m_DamageFont;

#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color" )
	float m_fCurrentOpacity = 1.0f;

	/*
protected:
	void NPCMeshLoadCompleted();
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;
	
	TSharedPtr<FStreamableHandle> NPCMeshHandle;
	*/
private:
	FTimerHandle m_hDeathTimer;
	FTimerHandle m_hAlertTimer;

#pragma region DEBUG

private:
	FTimerHandle m_hDebug;
	void Debug();

#pragma endregion

};
