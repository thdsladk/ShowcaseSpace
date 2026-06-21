// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASPlayerController.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "UI/GASHUD.h"
#include "UI/RadialMenuWidget.h"
#include "UI/GASCursorWidget.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"        // ASC 접근
#include "AbilitySystemInterface.h"        // IAbilitySystemInterface
#include "GameplayTagContainer.h"          // FGameplayTag, FGameplayTagContainer
//#include "GameplayTagsManager.h"           // RequestGameplayTag()
#include "GameFramework/PlayerState.h"     // APlayerState


#include "Character/GASPlayerCharacter.h"

AGASPlayerController::AGASPlayerController()
	:m_VisibleCursor(false)
{
	m_ClickActions_AbilityInputID.Reserve(int32(EInputAbilityID::End));


	// 마우스 커서 표시
	bShowMouseCursor = m_VisibleCursor; // true;

	//bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
	
	m_PlayerTeamId = FGenericTeamId(0);
	
}

void AGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = m_VisibleCursor; // true;

	/*
		순서가 중요하다 어빌리티 시스템 컴포넌트가 세팅되고 NativeConstruct가(뷰포트에 붙어서 화면에 뜰 때 실행된다. )  실행되도록 해야 한다.	
	*/
	m_HUD = CreateWidget<UGASHUD>(this, m_HUDClass);
	if(m_HUD != nullptr)
	{
		m_HUD->SetAbilitySystemComponent(GetPawn());
		m_HUD->SetOwningPlayer(this);
		m_HUD->AddToViewport();
	
	}

	m_CombatMenu = CreateWidget<URadialMenuWidget>(this, m_CombatMenuClass);
	if(m_CombatMenu != nullptr)
	{
		m_CombatMenu->SetOwningPlayer(this);
		m_CombatMenu->AddToViewport();
	}

	m_Menu = CreateWidget<UGASUserWidget>(this, m_MenuClass);
	if(m_Menu != nullptr)
	{
		m_Menu->SetOwningPlayer(this);
		//m_Menu->AddToViewport();
		
	}

	m_DeathWidget = CreateWidget<UGASUserWidget>(this, m_DeathWidgetClass);
	if (m_DeathWidget != nullptr)
	{
		m_DeathWidget->SetOwningPlayer(this);
		//m_DeathWidget->AddToViewport();
	}

	m_CursorWidget = CreateWidget<UGASCursorWidget>(this, m_CursorWidgetClass);
	if (m_CursorWidget != nullptr)
	{
		m_CursorWidget->SetOwningPlayer(this);
		m_CursorWidget->InitWidget();
		m_CursorWidget->AddToViewport(9999);	// 항상 위에 표시 되도록
	}

	//SetInputModeGameOnly();
	SetInputModeGameAndUI();

}

void AGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	
	if (IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Jump], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::Jump));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Jump], ETriggerEvent::Completed, this, &AGASPlayerController::GASInputReleased, int32(EInputAbilityID::Jump));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Attack], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::Attack));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Attack], ETriggerEvent::Completed, this, &AGASPlayerController::GASInputReleased, int32(EInputAbilityID::Attack));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Skill], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::Skill));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Sprint], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::Sprint));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Sprint], ETriggerEvent::Completed, this, &AGASPlayerController::GASInputReleased, int32(EInputAbilityID::Sprint));
		//EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::QuaterMove], ETriggerEvent::Started, this, &AGASPlayerController::GASInputQuateMove, int32(EInputAbilityID::QuaterMove));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Dodge], ETriggerEvent::Started, this, &AGASPlayerController::GASInputQuateMove, int32(EInputAbilityID::Dodge));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Dodge], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::Dodge));

		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::ChangeCombatMode], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::ChangeCombatMode));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::RangeAttack], ETriggerEvent::Started, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::RangeAttack));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::RangeAttack], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputReleased, int32(EInputAbilityID::RangeAttack));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Defense], ETriggerEvent::Started, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::Defense));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Defense], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputReleased, int32(EInputAbilityID::Defense));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::SkillQ], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::SkillQ));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::SkillE], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::SkillE));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::SkillR], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::SkillR));

		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::TargetOn], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::TargetOn));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::DashAttack], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputPressed, int32(EInputAbilityID::DashAttack));

		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Cancel], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputCancel, int32(EInputAbilityID::Cancel));
		EnhancedInputComponent->BindAction(m_ClickActions_AbilityInputID[EInputAbilityID::Confirm], ETriggerEvent::Triggered, this, &AGASPlayerController::GASInputConfirm, int32(EInputAbilityID::Confirm));


		/* 
			여러개를 바인딩 해두고 어빌리티에서 Tag로 구분해서 사용하는 식으로 구성. 
		*/
	}
}

void AGASPlayerController::GASInputPressed(int32 InputID) 
{
	if (m_ControlMode == EPlayerControlMode::NormalMode ||
		m_ControlMode == EPlayerControlMode::NoMovementInput ||
		m_ControlMode == EPlayerControlMode::NoSystemInput)
	{
		// 예외 처리 부분 특정모드에서 특정 인풋은 안되도록 
		//if (m_CombatMode == ECombatMode::RangeMode && InputID == 1)return;
		//if (m_CombatMode != ECombatMode::RangeMode && InputID == 6)return;


		CastChecked<AGASPlayerCharacter>(GetPawn())->GASInputPressed(InputID);
	}
}

void AGASPlayerController::GASInputReleased(int32 InputID)
{
	if (m_ControlMode == EPlayerControlMode::NormalMode ||
		m_ControlMode == EPlayerControlMode::NoMovementInput ||
		m_ControlMode == EPlayerControlMode::NoSystemInput)
	{
		// 예외 처리 부분 특정모드에서 특정 인풋은 안되도록 
		//if (m_CombatMode == ECombatMode::RangeMode && InputID == 1)return;
		//if (m_CombatMode != ECombatMode::RangeMode && InputID == 6)return;


		CastChecked<AGASPlayerCharacter>(GetPawn())->GASInputReleased(InputID);
	}
}

void AGASPlayerController::GASInputQuateMove(const FInputActionValue& Value, int32 InputID)
{
	if (m_ControlMode == EPlayerControlMode::NormalMode ||
		m_ControlMode == EPlayerControlMode::NoMovementInput ||
		m_ControlMode == EPlayerControlMode::NoSystemInput)
	{
		// 여기서는 방향만 입력 받는다.
		SetCachedInputVector(Value.Get<FVector2D>());

		//CastChecked<AGASPlayerCharacter>(GetPawn())->GASInputPressed(InputID);
		//CastChecked<AGASPlayerCharacter>(GetPawn())->GASInputQuateMove(Value.Get<FVector2D>(), InputID);
	}
}

void AGASPlayerController::GASInputConfirm(int32 InputID)
{
	CastChecked<AGASPlayerCharacter>(GetPawn())->GASInputConfirm(InputID);
}

void AGASPlayerController::GASInputCancel(int32 InputID)
{
	CastChecked<AGASPlayerCharacter>(GetPawn())->GASInputCancel(InputID);
}

void AGASPlayerController::UpdateCharacterControl()
{
	Super::UpdateCharacterControl();

}

void AGASPlayerController::SetInputModeGameOnly()
{
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
}
	
void AGASPlayerController::SetInputModeUIOnly()
{
	// 아직 미완성 
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AGASPlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void AGASPlayerController::ToggleMenu()
{
	m_VisibleMenu = ~m_VisibleMenu;
	if (m_VisibleMenu == true)
	{
		SetPause(m_VisibleMenu);
		m_Menu->AddToViewport();
		//SetInputModeUIOnly();
	}
	else
	{
		//(임시) 아직 일시정지 상황에서 입력이 작동하도록 제어하는 방법을 간구하지 못했다. 
		//SetPause(m_VisibleMenu);
		//m_Menu->RemoveFromViewport();
		//SetInputModeGameAndUI();
	}
}

void AGASPlayerController::ToggleDeathWidget()
{
	m_VisibleDeathWidget = ~m_VisibleDeathWidget;
	if (m_VisibleDeathWidget == true)
	{
		SetPause(m_VisibleDeathWidget);
		m_DeathWidget->AddToViewport();
	}
	else
	{
		//m_DeathWidget->RemoveFromViewport();
	}
}

FGameplayTag AGASPlayerController::GetCombatModeTag()
{
	APlayerState* PS = GetPlayerState<APlayerState>();
	if (PS != nullptr)
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PS);
		if (ASI != nullptr)
		{
			UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
			if (ASC != nullptr)
			{
				FGameplayTagContainer ActiveTags;
				TArray<FGameplayTag> MatchedTags;
				ASC->GetOwnedGameplayTags(ActiveTags);

				for (const FGameplayTag& Tag : ActiveTags)
				{
					if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Player.CombatMode"))) == true) // 부모 태그와 계층적으로 일치하는지 검사
					{
						return Tag;
					}
				}
				
			}
		}

	}
	


	return FGameplayTag();
}

FGenericTeamId AGASPlayerController::GetGenericTeamId() const
{
	return m_PlayerTeamId;
}
