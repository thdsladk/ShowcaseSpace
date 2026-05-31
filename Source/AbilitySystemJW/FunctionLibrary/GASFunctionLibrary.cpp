// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibrary/GASFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tag/JWGameplayTag.h"
#include "AbilitySystemComponent.h"

#include "Engine/OverlapResult.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Game/ProjectJWGameMode.h"

UAbilitySystemComponent* UGASFunctionLibrary::NativeGetASCFromActor(AActor* InActor)
{
    return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
}

void UGASFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UAbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UGASFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UAbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UGASFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UAbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);
    if (ASC != nullptr)
    {
        return ASC->HasMatchingGameplayTag(TagToCheck);
    }
    else
    {
        return false;
    }
}

void UGASFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EJWConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EJWConfirmType::Yes : EJWConfirmType::No;
}

UPawnCombatComponent* UGASFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
    //check(InActor);
    if(InActor == nullptr)
    {
        return nullptr;
    }

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    return nullptr;
}

UPawnCombatComponent* UGASFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EJWValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

    OutValidType = CombatComponent ? EJWValidType::Valid : EJWValidType::Invalid;

    return CombatComponent;
}

bool UGASFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

float UGASFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UGASFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

    // 두 벡터의 외적을 통해서 외적 결과벡터의 Z 값으로 회전 각도를 360도 전부 구한다. 
    if (CrossResult.Z < 0.f)
    {
        OutAngleDifference *= -1.f;
    }

    return FGameplayTag();
}
