// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_AbilityBase.h"
#include "GA_ActionAbilityBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FEndActionAbility)

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_ActionAbilityBase : public UGA_AbilityBase
{
	GENERATED_BODY()
public:
	UGA_ActionAbilityBase();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)override;


	FEndActionAbility EndActionAbility;					// 이 델리게이트는 가장 마지막(최말단 자식) EndAbility에서 실행되도록 하자. 

protected:
	UFUNCTION()
	void OnCompleteCallback_Montage();
	UFUNCTION()
	void OnInterruptedCallback_Montage();

	UFUNCTION(BlueprintCallable, Category = "LinkedAction")
	void PlayLinkedAction();


protected:
	UPROPERTY(EditAnywhere, Category = "GA|Flags")
	uint8 m_IsActive : 1;							// 액션 어빌리티 전용 활성화 플래그. 
	
	UPROPERTY(VisibleAnywhere, Category = "GA|InputIDs")
	int32 m_InputID =0;
#pragma region Montage Propertys
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GA|Montage")
	TObjectPtr<UAnimMontage> m_DefaultMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GA|Montage")
	FName m_SectionName;							// 아직은 단일 세션만을 재생하는 형태로 구현.


#pragma endregion

private:
	TObjectPtr<UAbilityTask_PlayMontageAndWait> m_PlayMontageTask;


};
