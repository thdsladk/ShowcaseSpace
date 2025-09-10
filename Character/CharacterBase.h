// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BehaviorInterface.h"
#include "Interface/InventoryInterface.h"
#include "Interface/TargetInterface.h"
#include "Header/BehaviorEnum.h"
#include "Header/CombatEnum.h"
//(임시)
#include "Header/DataStruct.h"
#include "GameplayTagContainer.h"

#include "CharacterBase.generated.h"

class UAnimInstanceBase;
class UMyStatComponent;
class UMyInventoryComponent;
class UMyEquipmentComponent;
class USkillComponent;
class UAbilityComponent;
class UAudioComponent;

class USoundCue;
class UMaterialInstanceDynamic;
class UNiagaraSystem;
class UMyAbilityData;



UCLASS()
class MYTEST_TOPDOWN_API ACharacterBase : public ACharacter, public IBehaviorInterface, public IInventoryInterface,public ITargetInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StateTag")
	TMap<EBehaviorState, FGameplayTag> StateTagMapping;

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

	virtual void SetupCharacterData();
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Components Part
public:
	class UMyStatComponent& GetStatComponent() { return *m_pStatComp; }

public:
	FORCEINLINE const int32 GetAttackIndex() { return m_AttackIndex; }

#pragma region Behavior Interface
public:
	// Behavior  컴파일 타임에 안쓸거라서 FORCEINLINE키워드를 없앤다. [ 런타임에 결정 ] 
	virtual const EBehaviorState GetState() { return m_CharacterState; }
	virtual void SetState(const EBehaviorState State); 
	virtual const EBehaviorState GetNextState() { return m_NextCharacterState; }
	virtual void SetNextState(const EBehaviorState State) { m_NextCharacterState = State; }
	virtual const ECharacterMode GetMode() { return m_Mode; }
	virtual void SetMode(const ECharacterMode Mode);

	//virtual const uint8 GetState() override { return static_cast<uint8>(m_CharacterState); }
	virtual void SetState(const uint8 State) override; 
	//virtual const uint8 GetNextState() override { return static_cast<uint8>(m_NextCharacterState); }
	virtual void SetNextState(const uint8 State) override { m_NextCharacterState = static_cast<EBehaviorState>(State); }

	//virtual const uint8 GetMode() override { return static_cast<uint8>(m_Mode); }
	virtual void SetMode(const uint8 Mode) override { m_Mode = static_cast<ECharacterMode>(Mode); }
	virtual const uint8 GetAggroGauge()override { return 0; };
	virtual void SetAggroGauge(const uint8 AggroGauge)override{};
	virtual const float GetAggroGauge_Ratio()override { return 0.f; };

#pragma region Combat Interface
	// Combat Stat 
	virtual const ECombatType GetCombatType()override { return m_CombatType; }
	virtual const float GetMeleeRadius() override { return m_AttackRadius + m_AttackRange; }// 공격 거리랑 반경을 합해야 공격할수 있는 반경이 나온다.
	virtual const float GetRangedRadius() override { return m_RangedRadius; }
	virtual const float GetDetectionRadius() override { return m_DetectionRadius; }
	virtual const float GetVisibleRadius() override { return m_VisibleRadius; }
#pragma endregion

	// Behavior Function
	virtual void OnIdle()override;
	virtual void OnBattle()override;
	virtual void Attack()override;
	virtual void AttackCheck();
	virtual void AttackEnd()override;
	virtual void Death()override;
	virtual void DeathEnd()override;
	virtual void OnHit()override;
	virtual void OnDefense()override;
	virtual void StopDefense()override;
	virtual void Defense_Hit()override;
	virtual void OnDetect()override;
	virtual void DetectEnd()override;
	virtual void OnAlert()override;

	virtual void OnCommand(uint8 Command)override;
	virtual void BehaviorInterrupted(uint8 State)override;

	virtual void LookDirection(float Value);

	virtual FOnChangeMode& GetChangeModeDelegate() { return OnChangeMode; };
	virtual FOnChangeCommand& GetChangeCommandDelegate() { return OnChangeCommand; };

	virtual void SetAttackDelegate(const FOnBehaviorAttackEnd& Delegate)override { OnAttackEnd = Delegate; }
	virtual void SetDefenseDelegate(const FOnBehaviorDefenseEnd& Delegate)override { OnDefenseEnd = Delegate; }
	virtual void SetDetectDelegate(const FOnBehaviorDetectEnd& Delegate)override { OnDetectEnd = Delegate; }
	virtual void SetAlertDelegate(const FOnBehaviorAlertEnd& Delegate)override { OnAlertEnd = Delegate; }
	virtual void SetDeathDelegate(const FOnBehaviorDeathEnd& Delegate)override { OnDeathEnd = Delegate; }

	virtual void SetChangeBehaviorDelegate(const FOnChangeBehavior& Delegate)override { OnChangeBehavior = Delegate; }
	virtual void SetUpdateBehaviorDelegate(const FOnUpdateBehavior& Delegate)override;
	virtual void SetChangeModeDelegate(const FOnChangeMode& Delegate) { OnChangeMode = Delegate; }

	virtual bool IsOnTriggerEscape()override;
#pragma endregion

#pragma region Delegate Member
	// 행동_끝 시점 관련 델리게이트
public:
	FOnBehaviorAttackEnd OnAttackEnd;
	FOnBehaviorDefenseEnd OnDefenseEnd;
	FOnBehaviorDetectEnd OnDetectEnd;
	FOnBehaviorAlertEnd OnAlertEnd;
	FOnBehaviorDeathEnd OnDeathEnd;

	FOnChangeBehavior OnChangeBehavior;
	FOnUpdateBehavior OnUpdateBehavior;
	FOnChangeMode OnChangeMode;
	FOnChangeCommand OnChangeCommand;


#pragma endregion

	// Effect
	void Effect_Flick(FLinearColor Color, float Rate = 0.1f );
	void EffectEnd_Flick();

#pragma region FootStep Function
public:
	virtual void PlayFootStep();
protected:
	void PlaySound_FootStep(int32 SurfaceIndex);
	void PlayEffect_FootStep(int32 SurfaceIndex,FVector Location,FRotator ActorRotator);

#pragma endregion

#pragma region Animation Function
public:
	FORCEINLINE const float GetAnimationSpeed() { return m_AnimationSpeed; }
	const float GetMoveAnimationSpeed();
	UFUNCTION()
	void OnStopAttackMontage(float InBlendOutTime = 0.f);
	UFUNCTION()
	void OnStopBehaviorMontage(float InBlendOutTime = 0.f);
	UFUNCTION()
	void SetAnimationSpeed(float Speed) { m_AnimationSpeed = Speed; }

	UFUNCTION()
	virtual void OnHitMontageEnded();
#pragma endregion

#pragma region Movement Function
public:
	// Movement Function
	virtual void MoveToForward(float Value = 1.f);
	virtual void MoveToRight(float Value = 1.f);
	void SetMoveSpeed(float WalkSpeed);
	void ResetMoveSpeed();
	void SetMoveSpeed_Ratio(float Ratio);

	virtual void Sprint(float WalkSpeedRatio = 2.f,float CameraSpeedRatio = 2.f, bool HasState = true);
	virtual void SetSprint(float WalkSpeed, float CameraSpeed = 1.f, bool HasState = true);
	virtual void StopSprint(bool HasState = true);

	UFUNCTION()
	const float GetHorizontal() { return m_Horizontal; }
	UFUNCTION()
	const float GetVertical() { return m_Vertical; }
	UFUNCTION()
	void SetHorizontal(float Horizontal) { m_Horizontal = Horizontal; }
	UFUNCTION()
	void SetVertical(float Vertical) { m_Vertical = Vertical; }
#pragma endregion
public:
	// CharacterBase 단에서 구분 할 수 있도록 제공해보자. 
	virtual bool IsPlayerCharacter() { return false; }

#pragma region Inventory Interface
public:
	// Inventory Function
	virtual const int32 GetItemType(int32 Index)override;

	virtual void UseItem(int32 Index)override;
	virtual void DropItem(int32 Index, FVector Pos = FVector(0.0, 0.0, 0.0))override;
	
	virtual bool Equip(uint8 Part) override;
	virtual bool UnEquip(uint8 Part) override;

	// Equipment Function
	void InitEquipment();
#pragma endregion

#pragma region Damage Function
public:
	// Damage Function
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE void SetInvincible(bool bInvincible) { m_bTakeDamage = !bInvincible; }
public:
	
	
	// Collision Function
protected:
	bool CheckLineTrace_Single(FVector Start,FVector End, ECollisionChannel Channel,float TransientDamage =0.f);
	bool CheckLineTrace_Multi(FVector Start,FVector End,ECollisionChannel Channel, float TransientDamage = 0.f);
	bool CheckSweep_Single(FCollisionShape CollisionShape, ECollisionChannel Channel, float TransientDamage = 0.f);
	bool CheckSweep_Multi(FCollisionShape CollisionShape, ECollisionChannel Channel, float TransientDamage = 0.f);
	bool CheckOverlap(FCollisionShape CollisionShape, ECollisionChannel Channel, float TransientDamage = 0.f);
#pragma endregion

#pragma region Target Interface
public:
	virtual const AActor* GetTarget()override { return(m_Targets.IsEmpty() ? nullptr : m_Targets[0].Get()); }
	virtual TArray<TWeakObjectPtr<AActor>>& GetTargets()override { return m_Targets; }
	virtual void AddTarget(AActor* Target)override { m_Targets.Emplace(Target); }
	virtual void AddTargets(TArray<AActor*> Targets)override { for (AActor* Target : Targets) { m_Targets.Emplace(Target); } }
	virtual void PopTarget()override { m_Targets.Pop(true); }
	virtual void ClearTargets()override { m_Targets.Empty(); }

#pragma endregion

#pragma region MeshComponent Function
public:
	void InitalizeMeshComponent();
	void SetMainMesh();

#pragma endregion

#pragma region ObjectPool Interface
public:
	// Render Section
	virtual void ApplyHidden(const bool bHide);
	// ObjectPool Setion
	virtual void AcquireObject() {};
	virtual void ReleaseObject() {};
#pragma endregion

#pragma region Components Section
protected:
	//	Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimInstanceBase> m_pAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UMyStatComponent> m_pStatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
	TObjectPtr<UMyInventoryComponent> m_pInventoryComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
	TObjectPtr<UMyEquipmentComponent> m_pEquipmentComp;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
	//TObjectPtr<USkillComponent> m_pSkillComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)

	TObjectPtr<UAbilityComponent> m_pAbilityComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
	TObjectPtr<UAudioComponent> m_pAudioComp;

	// 장비 메시 컴포넌트 < 반드시 캐릭터에게 들어가야한다. >
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TArray<TObjectPtr<USkeletalMeshComponent>> m_EquipmentMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TArray<FName> m_PartTypeList{ TEXT("Head_Socket"),TEXT("LeftHand_Socket"),TEXT("Chest_Socket"),TEXT("RightHand_Socket"),TEXT("LeftFoot_Socket"),TEXT("RightFoot_Socket") };
	const int32 m_ItemFirstIndex = 6;
#pragma endregion

#pragma region Material Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialList", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInstanceDynamic> m_TestMaterial;
	UPROPERTY(VisibleAnywhere, Category = "MaterialList")
	TObjectPtr<UMaterialInterface> m_Shiny;
	UPROPERTY(EditAnywhere, Category = "MaterialList")
	TObjectPtr<UMaterialInterface> m_OriginMaterial;
#pragma endregion

#pragma region FootStep Section
protected:
	// Effect
	UPROPERTY(EditAnywhere, Category = "Effects")
	TArray<UNiagaraSystem*> m_NiagaraEffects;

	UPROPERTY(EditAnywhere, Category = Audio)
	USoundCue* m_FootstepCue;
#pragma endregion

#pragma region Identifier Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
	FString m_CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
	int32 m_CharacterId;
#pragma endregion

protected:
	UPROPERTY()
	int32 m_AttackIndex = 0;
	UPROPERTY()
	float m_Vertical = 0.f;
	UPROPERTY()
	float m_Horizontal = 0.f;
	UPROPERTY()
	float m_OriginMoveSpeed = 0.f;
	UPROPERTY()
	float m_AnimationSpeed = 1.f;
	UPROPERTY()
	float m_OriginAnimationSpeed = 1.f;

#pragma region Combat Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	float m_AttackRange;										   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	float m_AttackRadius;										   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	float m_RangedRadius;										   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	float m_DetectionRadius;									   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	float m_VisibleRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	ECombatType m_CombatType = ECombatType::CT_None;

#pragma endregion

#pragma region Ability Section
protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TMap<FGameplayTag,TObjectPtr<UMyAbilityData>> m_AbilityData;

#pragma endregion
protected:
	// FSM				:: Next는 지연 시켜서 상태 처리할때 사용 :: 
	UPROPERTY(VisibleAnywhere, Category = "State")
	EBehaviorState m_NextCharacterState = EBehaviorState::Idle;
	UPROPERTY(VisibleAnywhere, Category = "State")
	EBehaviorState m_CharacterState = EBehaviorState::Idle;
	
	// Mode
	UPROPERTY(VisibleAnywhere, Category = "Mode")
	ECharacterMode m_Mode = ECharacterMode::Idle;

	// Render Section
	UPROPERTY(VisibleAnywhere, Category = Boolean)
	uint8 m_bVisiable : 1;

	// Damage Section
	UPROPERTY(VisibleAnywhere, Category = Boolean)
	uint8 m_bTakeDamage : 1;

	// Target Section
	UPROPERTY(VisibleAnywhere, Category = Targets)
	TArray<TWeakObjectPtr<AActor>> m_Targets;

	// Ally,Enemy Collision Channel
	//UPROPERTY(EditAnywhere, Category = CollisionChannel)
	ECollisionChannel m_Ally = ECollisionChannel::ECC_Visibility;
	//UPROPERTY(EditAnywhere, Category = CollisionChannel)
	ECollisionChannel m_Enemy= ECollisionChannel::ECC_Visibility;

#pragma region AggroGauge Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gauge", Meta = (AllowPrivateAccess = true))
	uint8 m_AggroGauge = 0;
public:
	FChangeAggroGauge m_ChangeAggroGauge;
#pragma endregion

#pragma region Timer Section
protected:
	FTimerHandle m_hEffectFlickTimer;
	FTimerHandle m_hDetectTimer;
	FTimerHandle m_hGaugeTimer;
#pragma endregion

};
