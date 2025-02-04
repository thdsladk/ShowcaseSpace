// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/MyMonsterWidgetInterface.h"
#include "Monster_Goblin.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnHitEnd);
DECLARE_MULTICAST_DELEGATE(FOnDefenseEnd);
DECLARE_MULTICAST_DELEGATE(FOnDetectEnd);
DECLARE_MULTICAST_DELEGATE(FOnAlertEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathEnd);


UCLASS()
class MYTEST_TOPDOWN_API AMonster_Goblin : public ACharacter ,public IMyMonsterWidgetInterface
{
	GENERATED_BODY()
public:
	enum EState : int8
	{
		Idle,
		Walk,
		Running,
		Chase,
		Attacking,
		Attacking_End,
		Hit,
		Defense,
		Stunned,
		Detect,
		Battle,
		Alert,
		Die,
		End

	};
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
		TEXT("Detect"),
		TEXT("Battle"),
		TEXT("Alert"),
		TEXT("Die"),
		TEXT("End") };

public:
	AMonster_Goblin(); 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	FORCEINLINE int8 GetState() { return m_State; }
	FORCEINLINE void SetState(int8 State) { m_State = State; }
	FORCEINLINE int8 GetNextState() { return m_NextState; }
	FORCEINLINE void SetNextState(int8 State) { m_NextState = State; }

	FORCEINLINE class UMonsterStatComponent* GetStatus() { return m_pStatComp; }

	// Behavior
	void Attack();
	void AttackCheck();
	void Death();
	void Death_End();
	void Attacked();
	void AttackEnd();
	void OnDefense();
	void Defense_Hit();

	void OnDetect();
	void OnAlert();

	// Effect
	void Effect_Flick(FLinearColor Color);
	void EffectEnd_Flick();

	// 모션_끝 시점 관련 델리게이트
	FOnAttackEnd OnAttackEnd;
	FOnHitEnd OnHitEnd;
	FOnDefenseEnd OnDefenseEnd;
	FOnDetectEnd OnDetectEnd;
	FOnAlertEnd OnAlertEnd;
	FOnDeathEnd OnDeathEnd;

	// Animation Function
	UFUNCTION()
	void StopMontage(int32 iState);
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnDeathMontageEnded();


	// Movement Function
	void MoveToFoward(float Value);
	void MoveToSide(float Value);
	void LookDirection(float Value);
	void SetMoveSpeed(float WalkSpeed);

	UFUNCTION()
		float GetHorizontal() { return m_Horizontal; }
	UFUNCTION()
		float GetVertical() { return m_Vertical; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	// Widget Section
	virtual void SetupMonsterWidget(class UMyUserWidget* InUserWidget) override;


protected:
	//S/ 부속 파트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterType")
	FName m_MonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterMesh")
	class AMyEquipment* m_Equipment1 {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterMesh")
	class AMyEquipment* m_Equipment2 {nullptr};


	// Components

	UPROPERTY(VisibleAnywhere)
	class UMonsterStatComponent* m_pStatComp;
	UPROPERTY(VisibleAnywhere)
	class UAnimInstance_Goblin* m_AnimInstance;
	UPROPERTY(VisibleAnywhere, Category = "WidgetComp")
	class UWidgetComponent* m_HpBar;
	UPROPERTY(VisibleAnywhere, Category = "WidgetComp")
	class UWidgetComponent* m_Emotion;

	// 클래스만 먼저 정의하고 생성해서 쓰는 방식 
	// WidgetComponent로 붙이는 방식과 다르게 사용  
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UUserWidget> m_DamageFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialList", Meta = (AllowPrivateAccess = true))
	UMaterialInterface* m_TestMaterial;
	UPROPERTY(VisibleAnywhere, Category = "MaterialList")
	UMaterialInterface* m_Shiny;
	UPROPERTY(VisibleAnywhere, Category = "MaterialList")
	UMaterialInterface* m_OriginMaterial;
	//E/ 부속 파트 

	UPROPERTY()
	int32 m_AttackIndex = 0;
	UPROPERTY()
	float m_Vertical = 0.f;
	UPROPERTY()
	float m_Horizontal = 0.f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color" )
	float m_fCurrentOpacity = 1.0f;

	// FSM 
	UPROPERTY(VisibleAnywhere, Category = "State")
	int8 m_NextState = EState::Idle;
	UPROPERTY(VisibleAnywhere,  Category = "State")
	int8 m_State = EState::Idle;

	// Combat Stat 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float m_AttackRange = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float m_AttackRadius = 50.f;


private:
	FTimerHandle m_hDeathTimer;
	FTimerHandle m_hAlertTimer;
	FTimerHandle m_hEffectFlickTimer;

#pragma region DEBUG

private:
	FTimerHandle m_hDebug;
	void Debug();

#pragma endregion

};
