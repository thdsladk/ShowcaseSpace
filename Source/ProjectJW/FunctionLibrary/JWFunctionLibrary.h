// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "JWFunctionLibrary.generated.h"

using JWFL = UJWFunctionLibrary;

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


#pragma region MotionWarping Functions
public:
	// ── 1계층: 프리미티브 ─────────────────────────────
	static class UMotionWarpingComponent* GetMotionWarpingComponent(AActor* InActor);

	// 액터 회전 + 컨트롤러 회전을 함께 설정 (3곳에서 반복되던 쌍)
	UFUNCTION(BlueprintCallable, Category = "GAS|Warp")
	static void FaceRotation(APawn* InPawn, FRotator InRotation);

	// 평면(XY) 방향 벡터
	static FVector GetPlanarDirection(const FVector& From, const FVector& To);

	// 루트모션이면 워프 타겟 등록, 아니면 회전(+선택적 Launch)으로 대체하는 공통 스켈레톤.
	// 반환값: 루트모션 경로를 탔으면 true
	static bool WarpOrMove(
		AActor* InActor,
		UAnimMontage* Montage,
		FName WarpTargetName,
		const FVector& TargetLocation,
		const FRotator& TargetRotation,
		const FVector& LaunchVelocity = FVector::ZeroVector);

	static void WarpKnockback(AActor* Self, const AActor* Instigator, FName WarpTargetName,
		UAnimMontage* Montage, float Force);

	static void RemoveWarpTarget(AActor* InActor, FName WarpTargetName);

#pragma endregion

};
