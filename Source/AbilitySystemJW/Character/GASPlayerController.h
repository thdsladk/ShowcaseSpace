// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MyPlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "GASPlayerController.generated.h"

class UGASHUD;
class URadialMenuWidget;
class UGASCursorWidget;
class UGASUserWidget;

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AGASPlayerController : public AMyPlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AGASPlayerController();

protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;

protected:
	void GASInputPressed(int32 InputID);
	void GASInputReleased(int32 InputID);
	void GASInputQuateMove(const FInputActionValue& Value, int32 InputID);
	void GASInputConfirm(int32 InputID);
	void GASInputCancel(int32 InputID);

public:
	virtual void UpdateCharacterControl() override;

public:
	UFUNCTION(BlueprintCallable, Category = "InputMode")
	void SetInputModeGameOnly();
	UFUNCTION(BlueprintCallable, Category = "InputMode")
	void SetInputModeUIOnly();
	UFUNCTION(BlueprintCallable, Category = "InputMode")
	void SetInputModeGameAndUI();

#pragma region PlayerController Interface
public:
	virtual void ToggleMenu()override;
	virtual void ToggleDeathWidget()override;

	virtual FGameplayTag GetCombatModeTag()override;
#pragma endregion

#pragma region IGenericTeamAgent Interface
public:
	virtual FGenericTeamId GetGenericTeamId()const override;
#pragma endregion

#pragma region Widget Members
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UGASHUD> m_HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<UGASHUD> m_HUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|CombatMenu")
	TSubclassOf<URadialMenuWidget> m_CombatMenuClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|CombatMenu")
	TObjectPtr<URadialMenuWidget> m_CombatMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Menu")
	TSubclassOf<UGASUserWidget> m_MenuClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Menu")
	TObjectPtr<UGASUserWidget> m_Menu;

	UPROPERTY(EditAnywhere, Category = "HUD | Cursor")
	TSubclassOf<UGASCursorWidget> m_CursorWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "HUD | Cursor")
	TObjectPtr<UGASCursorWidget> m_CursorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|DeathWidget")
	TSubclassOf<UGASUserWidget> m_DeathWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|DeathWidget")
	TObjectPtr<UGASUserWidget> m_DeathWidget;

#pragma endregion

protected:
	UPROPERTY(EditAnywhere, Category = "HUD | Cursor")
	uint8 m_VisibleCursor : 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> m_ForceCameraShakeClass;

	FGenericTeamId m_PlayerTeamId;
};
