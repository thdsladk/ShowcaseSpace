// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/MoveInterface.h"
#include "Interface/EquipmentInterface.h"
#include "Interface/CollisionTeamInterface.h"
#include "Header/GlobalEnum.h"
#include "CharacterBase.generated.h"


class UCameraComponent;
class UInputComponent;
class UAnimInstanceBase;
class UMotionWarpingComponent;

class UCharacterControlData;
class UDA_CollisionTeam;
class UTargetSystemComponent;
class UPhysicsMotionSystemComponent;
class UUserWidget;
class UGASDecalComponent;
class UJWMovementComponent;
class UPawnCombatComponent;
class ULinkedActionComponent;

class UNiagaraComponent;

UCLASS()
class PROJECTJW_API ACharacterBase : public ACharacter, public IMoveInterface, public IEquipmentInterface, public ICollisionTeamInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer);


protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame 틱 호출은 아직 필요 없음.
	//virtual void Tick(float DeltaTime) override;

#pragma region Getter Functions

public:
	virtual void SetCharacterControlData(const UCharacterControlData* CharacterControlData);
	UMotionWarpingComponent& GetMotionWarpingComp() { return *m_MotionWarpingComponent; }
	virtual UTargetSystemComponent* GetTargetSystemComponent() const { return nullptr; }
	virtual UPhysicsMotionSystemComponent* GetPhysicsMotionSystemComponent() const { return nullptr; }
	virtual UGASDecalComponent* GetGASDecalComponent() const { return nullptr; }
	virtual ULinkedActionComponent* GetLinkedActionComp() { return nullptr; }
	FORCEINLINE UJWMovementComponent* GetMovementComp() { return m_MovementComponent; }
	FORCEINLINE bool IsValid_MotionWarpingComp() { return (m_MotionWarpingComponent != nullptr ? true : false); }

public:
	UFUNCTION(BlueprintCallable, Category = "MotionWarping")
	UMotionWarpingComponent* BP_GetMotionWarpingComp() { return m_MotionWarpingComponent; }

#pragma endregion

#pragma region Setter Functions

public:
	// Set Functions
	
public:


#pragma endregion

#pragma region Movement Interface
public:
	virtual void ShoulderMove(const FVector2D& Value)override;
	virtual void ShoulderLook(const FVector2D& Value)override;
	virtual void QuaterMove(FVector2D Value)override;
	virtual void WhealInput(const float& Value)override;
	
	virtual void ReleaseMove(const FVector2D Value)override {}
	virtual void RotateToInterp(FVector2D Value, float InterpSpeed = 720.f)override;

public:
	virtual void StartSprint()override;
	virtual void StopSprint()override;
	virtual void SetSprint(float Ratio)override;

public:
	virtual void InitCameraSpeed()override{}
	virtual void SetCameraSpeed(float NewCameraSpeed)override{}
	virtual void ScaleCameraSpeed(float Scale)override {}
#pragma endregion

#pragma region Equipment Interface
public:
	virtual void Equip(EEquipmentSocketName SocketName, EEquipmentType Type = EEquipmentType::None)override {}
	virtual void UnEquip(EEquipmentSocketName SocketName, EEquipmentType Type = EEquipmentType::None)override {}

#pragma endregion

#pragma region Collision Interface 
public:
	virtual ECollisionChannel GetFriendlyCollisionTeambyChannel()override;
	virtual ECollisionChannel GetNeutralCollisionTeambyChannel()override;
	virtual ECollisionChannel GetHostileCollisionTeambyChannel()override;
#pragma endregion

#pragma region Damage Functions

	virtual void PlayDamageFont(float DamageAmount, FVector HitPoint = FVector::ZeroVector) {};


#pragma endregion

#pragma region GhostTrail Functions
public:
	UFUNCTION(BlueprintCallable, Category = "NiagaraSystem|GhostTrail")
	void ToggleGhostTrail();
	UFUNCTION(BlueprintCallable, Category = "NiagaraSystem|GhostTrail")
	bool IsActiveGhostTrail();

#pragma endregion




//  구분선 ( 위는 함수 아래는 멤버 변수 ) 

#pragma region Components Members
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UAnimInstanceBase> m_AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionWarping", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> m_MotionWarpingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMovement", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UJWMovementComponent> m_MovementComponent;

	

#pragma endregion

#pragma region Particle Members
	protected:
		UPROPERTY(EditAnywhere, Category = "NiagaraSystem")
		TObjectPtr<UNiagaraComponent> m_GhostTrailNiagaraSystem;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NiagaraSystem")
		TObjectPtr <UNiagaraComponent> m_FootStepNiagaraSystem;
#pragma endregion


#pragma region Collision Members
protected:
	UPROPERTY(EditAnywhere, Category = "CollisionTeam")
	TObjectPtr<UDA_CollisionTeam> m_CollisionTeamData;
#pragma endregion

// Dead Section
protected:
	virtual void SetDead();
	void PlayDeadMontage();
	virtual void DeathEnded() {};
	float DeadEventDelayTime = 5.0f;


#pragma region DamageFont Class

protected:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> m_DamageFontClass;

#pragma endregion


	UPROPERTY(EditAnywhere,Category = "Components|Flags")
	uint8 m_HasRootMotion : 1;


};
