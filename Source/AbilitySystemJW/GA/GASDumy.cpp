// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GASDumy.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Tag/JWGameplayTag.h"
#include "Abilities/GameplayAbility.h"

AGASDumy::AGASDumy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	m_ActionPeriod = 3.0f;
}

void AGASDumy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_RotatingMovement->bAutoActivate = false;
	m_RotatingMovement->Deactivate();

	//m_AbilitySystemComponent->InitAbilityActorInfo(this, this);
	//
	//for (const auto& StartAbility : m_StartAbilities)
	//{
	//	FGameplayAbilitySpec StartSpec(StartAbility);
	//	m_AbilitySystemComponent->GiveAbility(StartSpec);
	//}
}

void AGASDumy::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(m_ActionTimer, this, &AGASDumy::TimerAction, m_ActionPeriod, true, 0.0f);

}

void AGASDumy::TimerAction()
{
	FGameplayTagContainer TargetTag(JWTAG_ACTOR_ROTATE);

	//if (m_AbilitySystemComponent->HasMatchingGameplayTag(JWTAG_ACTOR_ISROTATING) == false)
	//{
	//	m_AbilitySystemComponent->TryActivateAbilitiesByTag(TargetTag);
	//}
	//else
	//{
	//	m_AbilitySystemComponent->CancelAbilities(&TargetTag);
	//}
}
