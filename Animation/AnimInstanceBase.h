// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Header/BehaviorEnum.h"
#include "Header/SkillEnum.h"
#include "AnimInstanceBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathEnd);
DECLARE_MULTICAST_DELEGATE(FOnSkillPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillEnd);
DECLARE_MULTICAST_DELEGATE(FOnHitEnd);
DECLARE_MULTICAST_DELEGATE(FOnStunEnd);
DECLARE_MULTICAST_DELEGATE(FOnAnimEnd);
DECLARE_MULTICAST_DELEGATE(FOnFootStep);


// EBehaviorState 는  "Header/BehaviorEnum.h"로 옮겨 뒀다.
// 고민을 해봐야한다 저위치가 맞는지...

UENUM(BlueprintType)
enum class EMontageList : uint8
{
	BehaviorMontage		UMETA(DisplayName = "BehaviorMontage"),
	AttackMontage		UMETA(DisplayName = "AttackMontage"),
	End					UMETA(DisplayName = "End")
};


/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAnimInstanceBase();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma region Getter Function
	FORCEINLINE const int32 GetMontageSectionNum(uint8 MontageIndex);
	FORCEINLINE const bool GetIsSprint() { return m_IsSprint; }
#pragma endregion

#pragma region Setter Function
	FORCEINLINE void SetIsSprint(bool IsSprint) { m_IsSprint = IsSprint; }
#pragma endregion
	/*
	void PlayBehaviorMontage(float PlaySpeed = 1.f);
	void StopBehaviorMontage(float InBlendOutTime = 0.f);
	void JumpToSection_Behavior(FName SectionName);

	void PlayAttackMontage(float PlaySpeed = 1.f);
	void StopAttackMontage(float InBlendOutTime = 0.f);
	void JumpToSection_Attack(uint8 SectionIndex);
	*/
	void PlaySkillMontage(uint8 SkillIndex, float PlaySpeed = 1.f);
	void StopSkillMontage(uint8 SkillIndex, float InBlendOutTime = 0.f);
	void JumpToSection_Skill(uint8 SkillIndex, FName SectionName);

	void PlayMontage(uint8 MontageIndex, float PlaySpeed = 1.f);
	void StopMontage(uint8 MontageIndex, float InBlendOutTime = 0.f);
	void JumpToSection(FName SectionName,uint8 MontageIndex);
	void StopAllMontage(float InBlendOutTime = 0.f);
	void PauseMontage(uint8 MontageIndex);

	const float GetPositionMontage(uint8 MontageIndex);
	void SetPositionMontage(uint8 MontageIndex, float NewPosition);
	void SetAutoBlendoutMontage(uint8 MontageIndex, bool bOnOff);

	FName GetAttackMontageName(int32 SectionIndex){return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));}

	FORCEINLINE void SetIsLive(bool IsLive) { m_IsLive = IsLive; }



	// 임시 함수 (DEBUG)
	const UAnimMontage* GetMontageInst(uint8 MontageIndex);


#pragma region Delegate Member
public:
	FOnAttackHit	m_OnAttackHit;
	FOnAttackEnd	m_OnAttackEnd;
	FOnDeathEnd		m_OnDeathEnd;
	FOnSkillPoint	m_OnSkillPoint;
	FOnSkillEnd		m_OnSkillEnd;
	FOnHitEnd		m_OnHitEnd;
	FOnStunEnd		m_OnStunEnd;
	FOnAnimEnd		m_OnAnimEnd;
	FOnFootStep		m_OnFootStep;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Vertical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	uint8 m_AttackIndex =0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	uint8 m_IsFalling : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	uint8 m_IsLive :1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	uint8 m_IsSprint :1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	uint8 m_IsAccelerating : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_AttackSpeedRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_MoveSpeedRate =1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState, Meta = (AllowPrivateAccess = true))
	EBehaviorState m_CharacterState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterMode, Meta = (AllowPrivateAccess = true))
	ECharacterMode m_CharacterMode;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> m_MontageList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> m_SkillMontageList;


protected:
	UFUNCTION()
	void AnimNotify_AnimEnd() { m_OnAnimEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_AttackHit() { m_OnAttackHit.Broadcast(); }
	UFUNCTION()
	void AnimNotify_AttackEnd() { m_OnAttackEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_DeathEnd() { m_OnDeathEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_SkillPoint() { m_OnSkillPoint.Broadcast(); }
	UFUNCTION()
	void AnimNotify_SkillEnd() { m_OnSkillEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_HitEnd() { m_OnHitEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_StunEnd() { m_OnStunEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_FootStep() { m_OnFootStep.Broadcast(); }

};
