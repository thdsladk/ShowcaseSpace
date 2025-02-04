// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillEnd);
DECLARE_MULTICAST_DELEGATE(FOnHitEnd);

/**
 * 
 */


UCLASS()
class MYTEST_TOPDOWN_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	enum EState : int8
	{
		Idle,
		Walk,
		Running,
		Attacking,
		Hit,
		Stunned,
		Casting,
		Die,
		End

	};

public:
	UMyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToSection_Attack(int32 SectionIndex);

	void PlayDeathMontage();
	void JumpToSection_Death(FName SectionName);
	
	void PlaySkillMontage();
	void JumpToSection_Skill(FName SectionName);

	void PlayHitMontage();
	void JumpToSection_Hit(FName SectionName);


	FName GetAttackMontageName(int32 SectionIndex);

	FOnAttackHit OnAttackHit;
	FOnAttackEnd OnAttackEnd;
	FOnDeathPoint OnDeathPoint;
	FOnSkillPoint OnSkillPoint;
	FOnSkillEnd OnSkillEnd;
	FOnHitEnd OnHitEnd;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Horizontal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsLive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int32 m_State;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* m_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* m_DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

private:
	UFUNCTION()
		void AnimNotify_AttackHit();
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_DeathPoint();
	UFUNCTION()
		void AnimNotify_SkillPoint();
	UFUNCTION()
		void AnimNotify_SkillEnd();
	UFUNCTION()
		void AnimNotify_HitEnd();
	UFUNCTION()
		void AnimTrail_Trail();

};
