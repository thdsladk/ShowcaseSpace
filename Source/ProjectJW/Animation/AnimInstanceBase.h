// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/AnimNotifyInterface.h"
#include "Header/GlobalEnum.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "AnimInstanceBase.generated.h"

class ACharacter;
class UCharacterMovementComponent;
class UAbilitySystemComponent;
class UComboActionData;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTJW_API UAnimInstanceBase : public UAnimInstance, public IAnimNotifyInterface
{
	GENERATED_BODY()


public:
	UAnimInstanceBase();
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

protected:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif // WITH_EDITOR
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	FORCEINLINE UAnimMontage* GetSkillActionMontage() const { return m_SkillActionMontage; }
	FORCEINLINE UAnimMontage* GetDeadMontage() const { return m_DeadMontage; }
public:
	FORCEINLINE UAnimMontage* GetCombatModeMontage(uint8 Mode) const { return m_CombatModeMontages[Mode]; }
	FORCEINLINE UAnimMontage* GetCombatModeMontage(ECombatMode Mode) const { return m_CombatModeMontages[uint8(Mode)]; }

	FORCEINLINE UAnimMontage* GetCurrentAttackMontage() const { return m_AttackMontages[uint8(m_CombatModeRef)]; }
	FORCEINLINE UAnimMontage* GetCurrentDefenseMontage() const { return m_DefenseMontages[uint8(m_CombatModeRef)]; }
	FORCEINLINE UAnimMontage* GetCurrentDodgeMontage() const { return m_DodgeMontages[uint8(m_CombatModeRef)]; }
	FORCEINLINE UAnimMontage* GetCurrentCombatModeMontage() const { return m_CombatModeMontages[uint8(m_CombatModeRef)]; }

	FORCEINLINE  UComboActionData* GetCurrentComboActionData() const { return m_ComboActionDatas[uint8(m_CombatModeRef)]; }

public:
	FORCEINLINE void SetViewAngle(float Angle) { m_ViewAngle = Angle; }
public:
	void PlayDeadMontage(float Rate);


public:
	virtual void BindEndMove(FOnEndMove Delegate)override;
	virtual void BindEndSection(FOnEndSection Delegate)override;
	virtual void BindShootPoint(FOnShootPoint Delegate)override;
	virtual void BindNotifyPoint(FOnNotifyPoint Delegate)override;
	
	virtual FOnDeathPoint& GetDeathPoint()override {return OnDeathPoint;}

#pragma region Delegate Section
protected:
	UFUNCTION()
	void AnimNotify_EndMove() { OnEndMove.Broadcast(); }
	UFUNCTION()
	void AnimNotify_EndSection() { OnEndSection.Broadcast(); }
	UFUNCTION()
	void AnimNotify_Shoot() { OnShootPoint.Broadcast(); }
	UFUNCTION()
	void AnimNotify_NotifyPoint() { OnNotifyPoint.Broadcast(); }

	UFUNCTION()
	void AnimNotify_DeathPoint() { OnDeathPoint.Broadcast(); }

protected:
	FOnEndMove OnEndMove;
	FOnEndSection OnEndSection;
	FOnShootPoint OnShootPoint;
	FOnNotifyPoint OnNotifyPoint;
	FOnDeathPoint OnDeathPoint;
#pragma endregion

#pragma region Property Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<ACharacter> m_Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> m_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector m_Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float m_GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float m_Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float m_ViewAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float m_MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float m_JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsJumping : 1;
			
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECombatMode m_CombatModeRef;

#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> m_DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> m_SkillActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> m_AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> m_DefenseMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> m_DodgeMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> m_CombatModeMontages;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UComboActionData>> m_ComboActionDatas;

	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
