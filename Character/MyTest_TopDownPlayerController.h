// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyTest_TopDownPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
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
		Key_Space,
		Key_Shift,
		Key_F,
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

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))
		void K2_OnScoreChanged(int32 NewScore);
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
		void K2_OnGameClear();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
		void K2_OnGameOver();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameRetryCountCpp"))
		void K2_OnGameRetryCount(int32 NewRetryCount);

	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void ClickRMouse();

	void Click_Tab();

	void Click_F();
	void Click_P();

	void Click_Q();
	void Click_W();
	void Click_E();
	void Click_R();
	void Click_V();
	void Click_Space();
	void Click_Shift();
	void Release_Space();
	void Release_Shift();



private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	uint8 m_UsedKeyNum = Key_End;

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
	class UInputAction* SetDestinationLClickAction;

	/** Right Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationRClickAction;

	/** Touch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationTouchAction;

	/**  Key Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<FKey, class UInputAction*> SetDestinationKeyAction;
	//TArray<class UInputAction*> SetDestinationKeyAction;
};


