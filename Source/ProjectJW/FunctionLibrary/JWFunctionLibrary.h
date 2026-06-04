// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "JWFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJW_API UJWFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool IsDebugMode(AActor* SourceActor);

#pragma region Collision Function
public:
	static bool CheckCollisionTrace_LineSingleByChannel(AActor* SourceActor, float Range, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_SweepSingleByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_SweepMultiByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FHitResult>& OutHitResults, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_OverlapMultiByChannel(AActor* SourceActor, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutOverlapResults, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_NearestOverlapByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutNearestFirstOverlapResults, FName TraceTag = FName(), bool bIgnoreSelf = true);
	
	static bool CheckCollisionTrace_LineSingleByChannel(AActor* SourceActor, const FVector Start,const FVector End, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_SweepSingleByChannel(AActor* SourceActor, const FVector Start, const FVector End, FCollisionShape Collision, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag = FName(),bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_SweepMultiByChannel(AActor* SourceActor, const FVector Start, const FVector End, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FHitResult>& OutHitResults, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_OverlapMultiByChannel(AActor* SourceActor, const FVector Start, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutOverlapResults, FName TraceTag = FName(), bool bIgnoreSelf = true);
	static bool CheckCollisionTrace_NearestOverlapByChannel(AActor* SourceActor, const FVector Start, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutNearestFirstOverlapResults, FName TraceTag = FName(), bool bIgnoreSelf = true);

	
	static void Debug_ShapeSingleByChannel(AActor* SourceActor, FVector Position, FCollisionShape Collision, bool HitDetected);
	static void Debug_ShapeSingleByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, bool HitDetected);
	static void Debug_ShapeSingleByChannel(AActor* SourceActor, const FVector Start, const FVector End, FCollisionShape Collision, bool HitDetected);

	static void Debug_Direction(AActor* SourceActor, const FVector Direction, float Length, FColor Color = FColor::Green, float Duration = 1.0f);
	
	// Visibility로 기본 설정을 하는 이유는 보이는 판정을 Default로 하기 위함
	static FVector ComputePositionFromMouse(APlayerController* PlayerController, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);
	static void ComputeRotationFromMouse(APlayerController* PlayerController, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);
	static FRotator ComputeLookAtRotationFromMouse(APlayerController* PlayerController, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);

	static FVector ComputePositionFromMouse(AActor* SourceActor, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);
	static void ComputeRotationFromMouse(AActor* SourceActor, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);
	static FRotator ComputeLookAtRotationFromMouse(AActor* SourceActor, ECollisionChannel Channel = ECollisionChannel::ECC_Visibility);

#pragma endregion


#pragma region GameplayTag Function
public:
	static TArray<FString> SplitTagToArray(const FGameplayTag& GameplayTag);
	static TArray<FString> GetTagPrefixArray(const FGameplayTag& GameplayTag, uint32 Index);
	static FString GetTagSegmentAt(const FGameplayTag& GameplayTag, uint32 Index);
	static FString GetTagSuffixString(const FGameplayTag& GameplayTag, uint32 Index);


#pragma endregion

};
