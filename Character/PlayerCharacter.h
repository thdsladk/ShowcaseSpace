// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interface/PlayerControllInterface.h"
#include "Interface/InteractableInterface.h"
#include "PlayerCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UAnimInstanceBase;
class UMyInventoryComponent;
class USkillComponent;
class UInputComponent;
class AController;

/*
	클래스 개요

	<플레이어 클래스>

	CharacterBase는 가지지 않는  PlayerCharacter만이 가지는 특징들을 선언하자.

	-플레이어 카메라 기능 
	-키입력 기능


 */
UCLASS(Blueprintable)
class MYTEST_TOPDOWN_API APlayerCharacter : public ACharacterBase, public IPlayerControllInterface, public IInteractableInterface
{
	GENERATED_BODY()
public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void SetupCharacterData()override;

public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void ChangeCameraView();

	/** Returns m_TopDownCameraComp subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return m_TopDownCameraComp; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }

#pragma region Behavior Function
public:
	// Behavior Section
	virtual const uint8 GetAggroGauge()override { return m_AggroGauge; };
	virtual void SetAggroGauge(const uint8 AggroGauge)override { m_AggroGauge = FMath::Clamp(m_AggroGauge + AggroGauge, 0, 100); }
	virtual const float GetAggroGauge_Ratio()override { return static_cast<float>(m_AggroGauge) / 100.f; };

	//virtual void AttackCheck()override;
	virtual void OnDefense()override;
	virtual void StopDefense()override;
	virtual void Death()override;
#pragma endregion

public:
	// ArmCamera Section 
	void Look_UpDown(float Value);
	void Look_LeftRight(float Value);
	void Wheel_UpDown(float value);

#pragma region Movement Function
public:
	// Movement Function (override)
	virtual void MoveToForward(float Value = 1.f)override;
	virtual void MoveToRight(float Value = 1.f)override;
	void MoveToDirection2D(const FVector2D& Direction);

	virtual	void Sprint(float WalkSpeedRatio = 2.f, float CameraSpeedRatio = 2.f, bool HasState = true)override;
	virtual	void SetSprint(float WalkSpeed, float CameraSpeed = 1.f, bool HasState = true)override;
	virtual	void StopSprint(bool HasState = true)override;
#pragma endregion

public:
	// HUD Function 
	UFUNCTION()
	void HUDUpdate(uint8 Type);

#pragma region Animation Function
public:
	// Animation Section
	UFUNCTION()
	void OnDeathMontageEnded();
	virtual void OnHitMontageEnded()override;
#pragma endregion

#pragma region Interact Interface
public:
	// Interact Section 
	virtual void ApplyInteraction();
	virtual void CheckForInteractable();
	virtual bool HasCurrentInteractable() { return (m_CurrentInteractable != nullptr); }
	virtual void SetCurrentInteractable(AMyInteractable* Interactable); 
#pragma endregion

#pragma region Controller Interface
	// Controller Interface
	virtual void ClickButton(uint8 Button)override;
	virtual void Click_F()override;
	virtual void Click_Q()override;
	virtual void Click_W()override;
	virtual void Click_E()override;
	virtual void Click_R()override;
	virtual void Click_V()override;
	virtual void Click_A()override;
	virtual void Click_S()override;
	virtual void Click_D()override;
	virtual void Click_G()override;
	virtual void Click_H()override;
	virtual void Click_J()override;
	virtual void Click_T()override;

	virtual void Click_Space()override;
	virtual void Click_Shift()override;
	virtual void ReleaseButton(uint8 Button)override;
	virtual void Release_Space()override;
	virtual void Release_Shift()override;
	virtual void ClickRMouse()override;

	virtual void Keyboard_Move(const FInputActionValue& Value)override;
	virtual void Look(const FInputActionValue& Value)override;

#pragma endregion

public:
	// Damage Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool IsPlayerCharacter() override { return true; }

	// Render Section
	virtual void ApplyHidden(const bool bHide);

#pragma region Components Member
protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> m_TopDownCameraComp;
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> m_CameraBoom;

#pragma endregion

protected:

	UPROPERTY()
	float m_OriginCameraSpeed = 0.f;

	// Interactable Section
	UPROPERTY()
	float m_InteractableReach = 1000.f;
	UPROPERTY()
	TWeakObjectPtr<AMyInteractable> m_CurrentInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString m_HelpText;

	// Camera Section
	UPROPERTY()
	uint8 m_bShoulderView : 1;

#pragma region DEBUG

private:
	FTimerHandle m_hDebug;
	void Debug();
	// Timer 
	FTimerHandle m_Timer_ItemCollision;
#pragma endregion
};
