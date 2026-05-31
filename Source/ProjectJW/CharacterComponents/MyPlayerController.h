// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Interface\PlayerControllerInterface.h"
#include "Header/GlobalEnum.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

class UCharacterControlData;


UENUM()
enum class ECharacterControlType : uint8
{
	//
	Shoulder			UMETA(DisplayName = "Shoulder"), 
	Shoulder_Sword		UMETA(DisplayName = "Shoulder_Sword"),
	Shoulder_Bow		UMETA(DisplayName = "Shoulder_Bow"),
	Shoulder_DualBlade	UMETA(DisplayName = "Shoulder_DualBlade"),

	//
	Quater				UMETA(DisplayName = "Quater"),
	Quater_Sword		UMETA(DisplayName = "Quater_Sword"),
	Quater_Bow			UMETA(DisplayName = "Quater_Bow"),
	Quater_DualBlade	UMETA(DisplayName = "Quater_DualBlade"),
	//

	End				UMETA(DisplayName = "End")

};

/**
 * 플레이어 컨트롤러는 GAS용으로 따로 만들지 말고 여기에 다 넣어버리자.
 * 
 */
UCLASS(Blueprintable)
class PROJECTJW_API AMyPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()
public:
	AMyPlayerController();

protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;
public:
	virtual void Tick(float DeltaSeconds) override;
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	
	void QuaterMove(const FInputActionValue& Value);
	void QuaterMouse(const FInputActionValue& Value);

	void ReleaseMove(const FInputActionValue& Value);

	void WhealInput(const FInputActionValue& Value);

#pragma region PlayerController Interface
public:
	virtual void UpdateCharacterControl() override;

	virtual const uint8 GetCombatMode() const override { return uint8(m_CombatMode); }
	virtual void SetCombatMode(uint8 Mode) override; 
	virtual const uint8 AddCombatMode() override
	{
		SetCombatMode((uint8(m_CombatMode) + 1U) % uint8(ECombatMode::End));
		return uint8(m_CombatMode);
	}

	virtual void SetControlMode(uint8 Mode) override { m_ControlMode = EPlayerControlMode(Mode); }
	virtual uint8 GetControlMode() const override { return uint8(m_ControlMode); }

	virtual const FVector2D& GetCachedInputVector()override;
	virtual void SetCachedInputVector(FVector2D Value)override;

	virtual const FVector2D& GetMouseMove()override { return m_CachedMouseMove; }
	virtual void SetMouseMove(FVector2D Value)override{ m_CachedMouseMove = Value; }


	virtual void PlayRotateCharacterToMouse(bool bTrace)override { m_IsRotateToMouse = bTrace; }
	virtual void RotateCharacterToMouse()override;

	virtual void ToggleMenu() override{}
	virtual void ToggleDeathWidget()override {}

	virtual FGameplayTag GetCombatModeTag()override;

#pragma endregion

#pragma region Delegate Events
	// 델리게이트를 public으로 노출해서 사용하고 있다. 
public:
	virtual FChangeCombatMode& OnChangeCombatMode()override { return ChangeCombatMode; }
	virtual FChangeMouseMove& OnChangeMouseMove()override { return ChangeMouseMove; }
protected:
	FChangeCombatMode ChangeCombatMode;
	FChangeMouseMove ChangeMouseMove;
#pragma endregion

protected:

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* m_DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_QuaterMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_WhealAction;


	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TMap<FName, TObjectPtr<UInputAction>> m_ClickActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TMap<EInputID, TObjectPtr<UInputAction>> m_ClickActions_InputID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TMap<EInputAbilityID,TObjectPtr<UInputAction>> m_ClickActions_AbilityInputID;

protected:
	ECharacterControlType m_CurrentCharacterControlType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECombatMode m_CombatMode;


	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, UCharacterControlData*> m_CharacterControlManager;

	EPlayerControlMode m_ControlMode;

	FVector2D m_CachedInputVector;
	FVector2D m_CachedMouseMove;

	UPROPERTY(EditAnywhere, Category = "CharacterRotate|Offset", Meta = (AllowPrivateAccess = "true"))
	float m_OffsetYaw;

	UPROPERTY(EditAnywhere, Category = CharacterRotate, Meta = (AllowPrivateAccess = "true"))
	uint8 m_IsRotateToMouse : 1;

#pragma region MenuWidget Members
protected:

	uint8 m_VisibleMenu : 1;
	uint8 m_VisibleDeathWidget : 1;
#pragma endregion


};
