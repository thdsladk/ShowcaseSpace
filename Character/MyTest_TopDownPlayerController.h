// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyTest_TopDownPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputAction;


DECLARE_MULTICAST_DELEGATE(FOnInvetory);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHUDUpdate, uint8);


UCLASS()
class AMyTest_TopDownPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/// <summary>
	/// 순서가 중요. ordered
	/// </summary>
	enum EKey : uint8
	{
		Key_Q,
		Key_W,
		Key_E,
		Key_R,
		Key_V,
		Key_P,
		Key_A,
		Key_S,
		Key_D,
		Key_Space,
		Key_Shift,
		Key_F,
		Key_G,
		Key_H,
		Key_J,
		Key_T,
		Key_Tab,
		Key_End
	};
	enum EHUDState : uint8
	{
		EIngame,
		EInventory,
		EShop,
		EStatus,
		ESkill,
		EEnd
	};

public:
	AMyTest_TopDownPlayerController();


	UUserWidget* GetCurrentWidget() { return m_CurrentWidget; }
	uint8 GetHUDState() { return m_HUDState; }
	void ApplyHUDChange();

	UFUNCTION(BlueprintCallable, Category = "HUD_Functions")
		void ChangeHUDState(uint8 State);

	bool ApplyHUD(TSubclassOf<UUserWidget> Widget, bool bShowMouse, bool EnableClickEvent);

	FOnHUDUpdate OnHUDUpdate;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	


public:
	FOnInvetory Inventory_Notify;

#pragma region GameMode Function
public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount);

	// Native Function
	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();
#pragma endregion

	// Set Controllable Section
	FORCEINLINE void SetGamePlayControllable(bool GameplayEnabled) { m_bGamePlayControllable = GameplayEnabled; }
	FORCEINLINE void SetSystemControllabel(bool SystemEnabled) { m_bSystemControllable = SystemEnabled; }

	FORCEINLINE void OnGamePlayControlReturned() { m_bGamePlayControllable = true; }

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint8 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

#pragma region Input Action
protected:
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

protected:
	// Click
	void ClickRMouse();
	void Click_Tab();
	void Click_F();
	void Click_P();
	void Click_Q();
	void Click_W();
	void Click_E();
	void Click_R();
	void Click_V();
	void Click_A();
	void Click_S();
	void Click_D();
	void Click_G();
	void Click_H();
	void Click_J();
	void Click_T();
	void Click_Space();
	void Click_Shift();
	// Release 
	void Release_Space();
	void Release_Shift();
	void Release_D();
#pragma endregion
	
	bool PerformLineTrace();

private:
	FVector m_CachedDestination;

	bool m_bIsTouch; // Is it a touch device
	float m_FollowTime; // For how long it has been pressed

	uint8 m_UsedKeyNum = Key_End;

	TWeakObjectPtr<AActor> m_TargetActor = nullptr;
	uint8 m_bGamePlayControllable : 1;
	uint8 m_bSystemControllable : 1;

protected:

	// HUD Section
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> HUD_Class;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> m_InventoryWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> m_ShopWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> m_StatusWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TObjectPtr<UUserWidget> m_CurrentWidget;

	uint8 m_HUDState = 0;

	// Key Action
	/** Left Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationLClickAction;

	/** Right Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationRClickAction;

	/** Touch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	/**  Key Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<FKey,UInputAction*> SetDestKeyAction;

};


