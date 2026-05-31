// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_RangeAttack.h"
//#include "Character/CharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimInstanceBase.h"
#include "Interface/PlayerControllerInterface.h"
#include "Interface/WidgetInterface.h"
#include "Prop/ProjectileBase.h"
#include "Components/TargetSystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "Tag/JWGameplayTag.h"

#include "AbilitySystemJW.h"

UGA_RangeAttack::UGA_RangeAttack()
	: m_ChargingGauge(0.f)
	, m_ShootState(uint8(EShootState::Stop))
	, m_IsInputRelease(false)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_RangeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
	IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Character->GetController());
	IWidgetInterface* WI = CastChecked<IWidgetInterface>(Character);

	PCI->PlayRotateCharacterToMouse(true);
	WI->SetRangeGaugeBar_Implementation(true);
	
	IncreaseState();

	// Bind Delegate
	FOnEndSection Delegate;
	Delegate.AddUObject(this, &UGA_RangeAttack::EndSection);
	AnimInstance->BindEndSection(Delegate);
	FOnShootPoint DelegateShoot;
	DelegateShoot.AddUObject(this, &UGA_RangeAttack::ShootProjectile);
	AnimInstance->BindShootPoint(DelegateShoot);


	m_PlayRangeAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRange"), AnimInstance->GetCurrentAttackMontage(), 1.0f, GetNextSection());
	m_PlayRangeAttackTask->OnCompleted.AddDynamic(this, &UGA_RangeAttack::OnCompleteCallback);
	m_PlayRangeAttackTask->OnInterrupted.AddDynamic(this, &UGA_RangeAttack::OnInterruptedCallback);
	m_PlayRangeAttackTask->ReadyForActivation();

}

void UGA_RangeAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(m_IsInputRelease == true)
	{
		return;
	}
	else
	{
		m_IsInputRelease = true;
	}

	if (m_ShootState == uint8(EShootState::Charge))
	{
		// 여기서 Charge -> Shot으로 넘어가서 중간 정지에 대한 처리를 해줘야한다.
		ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
		UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
		IncreaseState();
		AnimInstance->Montage_JumpToSection(GetNextSection(), GetCurrentMontage());

	}


}

void UGA_RangeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_RangeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	MontageStop(0.25f);

	m_ChargingGauge = 0.f;

	// Set Charging Gauge from AttributeSet
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (ASC != nullptr)
	{
		ASC->ApplyModToAttribute(
			UCharacterAttributeSet::GetChargeGaugeAttribute(), // Attribute
			EGameplayModOp::Override,                          // 연산 방식
			m_ChargingGauge                                    // 값
		);

		// Clear RangedCharge Effect
		ASC->RemoveActiveEffectsWithTags(FGameplayTagContainer{ JWTAG_CHARACTER_BUFF_RANGEDCHARGE });
	}
	
	m_IsInputRelease = false;

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Character->GetController());
	PCI->PlayRotateCharacterToMouse(false);

	IWidgetInterface* WI = CastChecked<IWidgetInterface>(Character);
	WI->SetRangeGaugeBar_Implementation(false);

}

void UGA_RangeAttack::OnCompleteCallback()
{
	//JWGAS_LOG(LogJWGAS, Log, TEXT("CurrentShootState : %d , Time : %lf"), m_ShootState,GetWorld()->GetRealTimeSeconds());
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_RangeAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
}

FName UGA_RangeAttack::GetNextSection()
{
	FName SectionName;
	switch (EShootState(m_ShootState))
	{
		case EShootState::Draw:
		{
			SectionName = FName("Draw");
			break;
		}
		case EShootState::Charge:
		{
			SectionName = FName("OverDraw");
			break;
		}
		case EShootState::Shot:
		{
			SectionName = FName("Shoot");
			break;
		}
		default:
		{
			SectionName = FName("");
			break;
		}
	}

	return SectionName;
}

void UGA_RangeAttack::EndSection()
{
	// 각 세션이 끝날때 마다 호출 

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());

	switch (m_ShootState)
	{
		case uint8(EShootState::Draw):
		{
			// 키가 해제 되었으면 Shot으로 넘어가고 아니면 Charge로 넘어가야한다. 
			if (m_IsInputRelease == true)
			{
				m_ShootState = uint8(EShootState::Shot);
				AnimInstance->Montage_JumpToSection(GetNextSection(), GetCurrentMontage());
			}
			else
			{
				IncreaseState();

				// Send GameplayEffect to increase Gauge
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(m_GaugeEffectClass);
				if (SpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle);
				}
			}
			break;
		}
		case uint8(EShootState::Charge):
		{
			IncreaseState();

			GetWorld()->GetTimerManager().ClearTimer(m_ChargingTimerHandle);
			break;
		}
		case uint8(EShootState::Shot):
		{
			IncreaseState();
			break;
		}
		default:
		{
			break;
		}
	}
}

void UGA_RangeAttack::ShootProjectile()
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());

	if (UAnimMontage* Montage = GetCurrentMontage())
	{
		const int32 SectionIndex = Montage->GetSectionIndex(FName("OverDraw"));
	
		if (SectionIndex != INDEX_NONE)
		{
			// Set Charging Gauge from AttributeSet
			UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
			m_ChargingGauge = ASC->GetSet<UCharacterAttributeSet>()->GetChargeGauge();
		}

		const FName SocketName = TEXT("SpawnPoint_Arrow");
		// 소켓 위치/회전
		bool Test = Character->GetMesh()->DoesSocketExist(SocketName);
		const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(SocketName);

		// 타겟 시스템 얻어오기.
		const UTargetSystemComponent* TargetSystemComp = Character->FindComponentByClass<UTargetSystemComponent>();
		if (TargetSystemComp == nullptr) return;
		
		AActor* TargetActor = TargetSystemComp->GetTargetActor();
		// (임시) 하드코딩으로 활 소켓 방향 잡아주고 있다...
		// Z축(Yaw 회전축) 기준으로 회전 쿼터니언 생성

		FRotator ResultRotator = UJWFunctionLibrary::ComputeLookAtRotationFromMouse(Character);
		FVector ProjectileDirection = ResultRotator.Vector();

		if (TargetActor != nullptr)
		{
			const FVector Target_Head = CastChecked<ACharacter>(TargetActor)->GetMesh()->GetSocketLocation(FName(TEXT("Head")));
			const FVector Character_LeftHand = Character->GetMesh()->GetSocketLocation(SocketName);
			ProjectileDirection.Z = (Target_Head - Character_LeftHand).GetSafeNormal().Z;
			
			AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(m_ProjectileClass, SpawnLocation,
				ResultRotator);
			if (Projectile != nullptr)
			{
				Projectile->SetOwner(Character);
				Projectile->FireInDirection(ProjectileDirection, m_ChargingGauge);
			}
		}
		else
		{

			AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(m_ProjectileClass, SpawnLocation,
				ResultRotator);
			if (Projectile != nullptr)
			{
				Projectile->SetOwner(Character);
				Projectile->FireInDirection(ProjectileDirection, m_ChargingGauge);
			}
		}
	}
}
