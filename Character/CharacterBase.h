// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BehaviorInterface.h"
#include "Header/BehaviorEnum.h"
#include "CharacterBase.generated.h"

class UMyStatComponent;

UENUM(BlueprintType)
enum class ECharacterMode : uint8 { Idle, Battle, StopAI, End };

// EBehaviorState 는  "Header/BehaviorEnum.h"로 옮겨 뒀다.
// 고민을 해봐야한다 저위치가 맞는지...

UCLASS()
class MYTEST_TOPDOWN_API ACharacterBase : public ACharacter, public IBehaviorInterface
{
	GENERATED_BODY()
public:
	TArray<FString> strState{
		TEXT("Idle"),
		TEXT("Walk"),
		TEXT("Running"),
		TEXT("Chase"),
		TEXT("Attacking"),
		TEXT("Attacking_End"),
		TEXT("Hit"),
		TEXT("Defense"),
		TEXT("Stunned"),
		TEXT("Casting"),
		TEXT("Detect"),
		TEXT("Battle"),
		TEXT("Alert"),
		TEXT("Die"),
		TEXT("End") };

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// Components Part
public:
	class UMyStatComponent& GetStatComponent() { return *m_pStatComp; }



	// Behavior  컴파일 타임에 안쓸거라서 FORCEINLINE키워드를 없앤다. [ 런타임에 결정 ] 
	virtual const uint8 GetState() override { return static_cast<uint8>(m_CharacterState); }
	virtual void SetState(const uint8 State) override { m_CharacterState = static_cast<EBehaviorState>(State); }
	virtual const uint8 GetNextState() override { return static_cast<uint8>(m_NextCharacterState); }
	virtual void SetNextState(const uint8 State) override { m_NextCharacterState = static_cast<EBehaviorState>(State); }

	virtual const uint8 GetMode() override { return static_cast<uint8>(m_Mode); }

	// Combat Stat 
	virtual const float GetMeleeRadius() override { return m_AttackRadius + m_AttackRange; }// 공격 거리랑 반경을 합해야 공격할수 있는 반경이 나온다.
	virtual const float GetRangedRadius() override { return m_DefenseRadius; }
	virtual const float GetDetectionRadius() override { return m_DetectionRadius; }
	virtual const float GetVisibleRadius() override { return m_VisibleRadius; }

	// Behavior
	virtual void OnIdle()override;
	virtual void OnBattle()override;
	virtual void Attack()override;
	virtual void AttackCheck();
	virtual void AttackEnd();
	virtual void Death()override;
	virtual void DeathEnd();
	virtual void OnHit()override;
	virtual void OnDefense()override;
	virtual void StopDefense()override;
	virtual void Defense_Hit()override;
	virtual void OnDetect()override;
	virtual void OnAlert()override;

	virtual void OnCommand(uint8 Command)override;

	virtual void LookDirection(float Value);

	virtual void SetAttackDelegate(const FOnBehaviorAttackEnd& Delegate)override { OnAttackEnd = Delegate; }
	virtual void SetDefenseDelegate(const FOnBehaviorDefenseEnd& Delegate)override { OnDefenseEnd = Delegate; }
	virtual void SetDetectDelegate(const FOnBehaviorDetectEnd& Delegate)override { OnDetectEnd = Delegate; }
	virtual void SetAlertDelegate(const FOnBehaviorAlertEnd& Delegate)override { OnAlertEnd = Delegate; }
	virtual void SetDeathDelegate(const FOnBehaviorDeathEnd& Delegate)override { OnDeathEnd = Delegate; }

	virtual void SetUpdateBehaviorDelegate(const FOnUpdateBehavior& Delegate)override;

	virtual bool IsOnTriggerEscape()override;

#pragma region Delegate Member
	// 행동_끝 시점 관련 델리게이트
	FOnBehaviorAttackEnd OnAttackEnd;
	FOnBehaviorDefenseEnd OnDefenseEnd;
	FOnBehaviorDetectEnd OnDetectEnd;
	FOnBehaviorAlertEnd OnAlertEnd;
	FOnBehaviorDeathEnd OnDeathEnd;

	FOnUpdateBehavior OnUpdateBehavior;
#pragma endregion

	// Effect
	void Effect_Flick(FLinearColor Color);
	void EffectEnd_Flick();



	// Movement Function
	virtual void MoveToFoward(float Value);
	virtual void MoveToRight(float Value);
	void SetMoveSpeed(float WalkSpeed);
	void ResetMoveSpeed();
	void SetMoveSpeed_Ratio(float Ratio);

	virtual void Sprint();
	virtual void SetSprint(float WalkSpeed, float CameraSpeed = 1.f);
	virtual void StopSprint();

	UFUNCTION()
	const float GetHorizontal() { return m_Horizontal; }
	UFUNCTION()
	const float GetVertical() { return m_Vertical; }
	UFUNCTION()
	void SetHorizontal(float Horizontal) { m_Horizontal = Horizontal; }
	UFUNCTION()
	void SetVertical(float Vertical) { m_Vertical = Vertical; }

	// CharacterBase 단에서 구분 할 수 있도록 제공해보자. 
	virtual bool IsPlayerCharacter() = 0;

protected:

	//	Components
	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<UMyStatComponent> m_pStatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialList", Meta = (AllowPrivateAccess = true))
		UMaterialInterface* m_TestMaterial;
	UPROPERTY(VisibleAnywhere, Category = "MaterialList")
		UMaterialInterface* m_Shiny;
	UPROPERTY(VisibleAnywhere, Category = "MaterialList")
		UMaterialInterface* m_OriginMaterial;

	UPROPERTY()
		int32 m_AttackIndex = 0;
	UPROPERTY()
		float m_Vertical = 0.f;
	UPROPERTY()
		float m_Horizontal = 0.f;
	UPROPERTY()
		float m_OriginMoveSpeed = 0.f;


	// Combat Stat 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_AttackRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_DefenseRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_DetectionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_VisibleRadius;

	// FSM				:: Next는 지연 시켜서 상태 처리할때 사용 :: 
	UPROPERTY(VisibleAnywhere, Category = "State")
	EBehaviorState m_NextCharacterState = EBehaviorState::Idle;
	UPROPERTY(VisibleAnywhere, Category = "State")
	EBehaviorState m_CharacterState = EBehaviorState::Idle;
	
	// Mode
	UPROPERTY(VisibleAnywhere, Category = "Mode")
	ECharacterMode m_Mode = ECharacterMode::Idle;

protected:
	FTimerHandle m_hEffectFlickTimer;
};
