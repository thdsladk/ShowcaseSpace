// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Goblin.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathPoint);

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UAnimInstance_Goblin : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstance_Goblin();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToSection(int32 SectionIndex);
	void JumpToSection_Death(FString str);

	void PlayDeathMontage();
	void PlayDeathMontage(float fTime);


	void StopMontage(int8 MontageIndex);
	void PauseMontage(int8 MontageIndex);
	const float GetPositionMontage(int8 MontageIndex);
	void SetPositionMontage(int8 MontageIndex, float fTime);
	void SetAutoBlendoutMontage(int8 MontageIndex,bool bOnOff);

	FName GetAttackMontageName(int32 SectionIndex);
	FName GetDeathMontageName(FString MontageName);

	FORCEINLINE void SetIsLive(bool IsLive) { m_IsLive = IsLive; }

	// 임시 함수 (DEBUG)
	const UAnimMontage* GetMontageInst(int8 MontageIndex);

	FOnAttackHit OnAttackHit;
	FOnAttackEnd OnAttackEnd;
	FOnDeathPoint OnDeathPoint;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_IsLive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	uint8 m_State =0;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* m_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* m_DeathMontage;
public:
	enum MontageList : int8
	{
		AttackMontage,
		DeathMontage,
		End
	};
private:
	UFUNCTION()
	void AnimNotify_AttackHit();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_DeathPoint();
};
