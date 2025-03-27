// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Header/BehaviorEnum.h"
#include "AnimInstanceBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillEnd);
DECLARE_MULTICAST_DELEGATE(FOnHitEnd);


// EBehaviorState 는  "Header/BehaviorEnum.h"로 옮겨 뒀다.
// 고민을 해봐야한다 저위치가 맞는지...

UENUM(BlueprintType)
enum class EMontageList : uint8
{
	AttackMontage		UMETA(DisplayName = "AttackMontage"),
	DeathMontage		UMETA(DisplayName = "DeathMontage"),
	SkillMontage		UMETA(DisplayName = "SkillMontage"),
	HitMontage			UMETA(DisplayName = "HitMontage"),
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

	void PlayAttackMontage(float PlaySpeed = 1.f);
	void StopAttackMontage(float InBlendOutTime = 0.f);
	void JumpToSection_Attack(int32 SectionIndex);

	void PlayDeathMontage(float PlaySpeed = 1.f);
	void StopDeathMontage(float InBlendOutTime = 0.f);
	void JumpToSection_Death(FName SectionName);

	void PlaySkillMontage(float PlaySpeed = 1.f);
	void StopSkillMontage(float InBlendOutTime = 0.f);
	void JumpToSection_Skill(FName SectionName);

	void PlayHitMontage(float PlaySpeed = 1.f);
	void StopHitMontage(float InBlendOutTime = 0.f);
	void JumpToSection_Hit(FName SectionName);

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
	FOnAttackHit	m_OnAttackHit;
	FOnAttackEnd	m_OnAttackEnd;
	FOnDeathPoint	m_OnDeathPoint;
	FOnSkillPoint	m_OnSkillPoint;
	FOnSkillEnd		m_OnSkillEnd;
	FOnHitEnd		m_OnHitEnd;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Vertical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_IsLive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EBehaviorState m_CharacterState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> m_MontageList;


private:
	UFUNCTION()
	void AnimNotify_AttackHit() { m_OnAttackHit.Broadcast(); }
	UFUNCTION()
	void AnimNotify_AttackEnd() { m_OnAttackEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_DeathPoint() { m_OnDeathPoint.Broadcast(); }
	UFUNCTION()
	void AnimNotify_SkillPoint() { m_OnSkillPoint.Broadcast(); }
	UFUNCTION()
	void AnimNotify_SkillEnd() { m_OnSkillEnd.Broadcast(); }
	UFUNCTION()
	void AnimNotify_HitEnd() { m_OnHitEnd.Broadcast(); }
	//UFUNCTION()
	//void AnimTrail_Trail() { m_OnTrail.Broadcast(); }
};
