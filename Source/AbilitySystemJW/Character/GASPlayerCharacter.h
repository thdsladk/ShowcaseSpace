// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Header/GlobalEnum.h"
#include "Interface/PawnCombatInterface.h"
#include "Interface/WidgetInterface.h"
#include "Interface/AbilityManagementInterface.h"

#include "GameplayAbilitySpec.h"          // FGameplayAbilitySpecHandle
#include "GameplayEffectTypes.h"          // FActiveGameplayEffectHandle

#include "GASPlayerCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UInputComponent;
class USkeletalMesh;

class UGASWidgetComponent;
class UDA_StartupCharacter;
class UDA_StartupCharacterSkill;
class UPlayerCombatComponent;

USTRUCT(BlueprintType)
struct FSkillAbilityHandleList
{
	GENERATED_BODY()
public:
	TArray<FGameplayAbilitySpecHandle> SkillAbilitySpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputSkillAbilitySpecHandles;
	TArray<FActiveGameplayEffectHandle>  StartupEffectSpecHandles;
};
/**
 * 
 */
UCLASS(Blueprintable)
class ABILITYSYSTEMJW_API AGASPlayerCharacter : public  APlayerCharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IWidgetInterface, public IAbilityManagementInterface
{
	GENERATED_BODY()
	
public:
	AGASPlayerCharacter(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay()override;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

#pragma region GetSet Functions
public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return m_PlayerCombatComponent; }
	virtual UTargetSystemComponent* GetTargetSystemComponent() const override{ return m_TargetSystemComponent; }
	virtual UPhysicsMotionSystemComponent* GetPhysicsMotionSystemComponent() const override{ return m_PhysicsMotionSystemComponent; }
	virtual UGASDecalComponent* GetGASDecalComponent() const override { return m_GASDecalComponent; }
	virtual ULinkedActionComponent* GetLinkedActionComp() { return m_LinkedActionComponent; }

public:
	UFUNCTION(BlueprintCallable, Category = "TargetSystem")
	UTargetSystemComponent* BP_GetTargetSystemComponent() { return GetTargetSystemComponent(); }
	UFUNCTION(BlueprintCallable, Category = "Decal")
	UGASDecalComponent* BP_GetGASDecalComponent() { return GetGASDecalComponent(); }

#pragma endregion

#pragma region Movement Interface
public:
	virtual void ShoulderMove(const FVector2D& Value)override;
	virtual void ShoulderLook(const FVector2D& Value)override;
	virtual void QuaterMove(FVector2D Value)override;

	virtual void ReleaseMove(const FVector2D Value)override;

#pragma endregion

public:
	void GASInputPressed(int32 InputID);
	void GASInputReleased(int32 InputID);
	//void GASInputQuateMove(const FVector2D& Value, int32 InputID);
	void GASInputConfirm(int32 InputID);
	void GASInputCancel(int32 InputID);

	UFUNCTION()
	virtual void OnOutOfHealth();
#pragma region Death Function
protected:	
	virtual void DeathEnded()override;
#pragma endregion

#pragma region Equipment Interface
public:
	virtual void Equip(EEquipmentSocketName SocketName, EEquipmentType Type = EEquipmentType::None)override;
	virtual void UnEquip(EEquipmentSocketName SocketName, EEquipmentType Type = EEquipmentType::None)override;

#pragma endregion

#pragma region Damage Functions

	virtual void PlayDamageFont(float DamageAmount, FVector HitPoint = FVector::ZeroVector)override;


#pragma endregion

#pragma region GameplayTag Function
public:
	const FGameplayTag GetCombatModeTag() const;
#pragma endregion

#pragma region Ability Management Interface
public:
	virtual void GrantSkillAbility(uint8 CombatMode)override;
	virtual void RemoveSkillAbility()override;
	virtual void ChangeSkillAbility(uint8 CombatMode)override;
	
#pragma endregion

#pragma region PawnCombat Interface
public:
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
#pragma endregion

#pragma region Widget Interface
public:
	virtual void SetRangeGaugeBar_Implementation(bool bVisible) override;
	virtual void SetVisibleGaugeBar_Implementation(bool bVisible) override {}

#pragma endregion


	////////////////////			Member				//////////////////////	

#pragma region Component Members
protected:
	/** Ability System */	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Abilities")
	TObjectPtr <UAbilitySystemComponent> m_AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|CombatSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCombatComponent> m_PlayerCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|TargetSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTargetSystemComponent> m_TargetSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|PhysicsMotionSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsMotionSystemComponent> m_PhysicsMotionSystemComponent;

	UPROPERTY(EditAnywhere, Category = "Components|DecalSystem")
	TObjectPtr<UGASDecalComponent> m_GASDecalComponent;
	
	UPROPERTY(EditAnywhere, Category = "Components|LinkedActionSystem")
	TObjectPtr<ULinkedActionComponent> m_LinkedActionComponent;
		

#pragma endregion

#pragma region Widget Components
protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetComp", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UGASWidgetComponent> m_GaugeBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetComp", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGASWidgetComponent> m_RangeGaugeBar;

protected:
	UPROPERTY(EditAnywhere, Category = "Widget|Value")
	FVector2D m_GaugeBarToolSize;

#pragma endregion

#pragma region Weapon MeshData
protected:

	TMap<EEquipmentSocketName, FName> m_EquipmentSocketNames;
	TMap<EEquipmentType, FName> m_EquipmentBackSocketNames;

#pragma endregion

#pragma region Ability System Members
protected:

	UPROPERTY(EditAnywhere, Category = "GAS|AbilityData")
	TSoftObjectPtr<UDA_StartupCharacter> m_StartupCharacterData;

	UPROPERTY(EditAnywhere, Category = "GAS|AbilityData")
	TMap<ECombatMode,TSoftObjectPtr<UDA_StartupCharacterSkill>> m_StartupCharacterSkillData;

	TObjectPtr< UDA_StartupCharacter> m_StartupCharacterDataObject;
	TMap < ECombatMode, TObjectPtr< UDA_StartupCharacterSkill>> m_StartupCharacterSkillDataObject;

	FSkillAbilityHandleList m_CurrentSkillAbilityList;

#pragma endregion

};
