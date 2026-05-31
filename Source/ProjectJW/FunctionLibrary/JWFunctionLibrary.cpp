// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/JWFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Engine/OverlapResult.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Game/ProjectJWGameMode.h"



bool UJWFunctionLibrary::IsDebugMode(AActor* SourceActor)
{
	AProjectJWGameMode* GameMode = Cast<AProjectJWGameMode>(SourceActor->GetWorld()->GetAuthGameMode());
    if (GameMode != nullptr)
    {
		return GameMode->IsDebugMode();
    }
    return false;
}

bool UJWFunctionLibrary::CheckCollisionTrace_LineSingleByChannel(AActor* SourceActor, float Range, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag, bool bIgnoreSelf)
{
    FVector Start = SourceActor->GetActorLocation();
    FVector End = Start + (SourceActor->GetActorForwardVector() * Range);

    // 충돌 파라미터
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);

    // 라인 트레이스 실행
    bool bHit = SourceActor->GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        Start,
        End,
        TraceChannel,
        Params
    );

    return bHit;
}

bool UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag, bool bIgnoreSelf)
{
    // 루트 컴포넌트가 캡슐 컴포넌트라는 전제하에 가능. 
    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(SourceActor->GetRootComponent());
    if (Capsule != nullptr)
    {
        float Radius = Capsule->GetScaledCapsuleRadius();
        FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);
        const FVector Forward = SourceActor->GetActorForwardVector();
        const FVector Start = SourceActor->GetActorLocation() + Forward * Radius;
        const FVector End = Start + Forward * Range;

        bool HitDetected = SourceActor->GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, TraceChannel, Collision, Params);

        return HitDetected;
    }

    return false;
}

bool UJWFunctionLibrary::CheckCollisionTrace_SweepMultiByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FHitResult>& OutHitResults, FName TraceTag, bool bIgnoreSelf)
{
    // 루트 컴포넌트가 캡슐 컴포넌트라는 전제하에 가능. 
    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(SourceActor->GetRootComponent());
    if (Capsule != nullptr)
    {
        float CapsuleRadius = Capsule->GetScaledCapsuleRadius();
        FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);
        const FVector Forward = SourceActor->GetActorForwardVector();
        const FVector Start = SourceActor->GetActorLocation() + Forward * CapsuleRadius;
        const FVector End = Start + Forward * Range;

        bool HitDetected = SourceActor->GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, TraceChannel, Collision, Params);

        return HitDetected;
    }
    return false;
}

bool UJWFunctionLibrary::CheckCollisionTrace_OverlapMultiByChannel(AActor* SourceActor, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutOverlapResults, FName TraceTag, bool bIgnoreSelf)
{
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);
    FVector OverlapOrigin = SourceActor->GetActorLocation();

    bool bHitDetected = SourceActor->GetWorld()->OverlapMultiByChannel(
        OutOverlapResults,
        OverlapOrigin,
        FQuat::Identity,
        TraceChannel,
        Collision,
        Params
    );

    return bHitDetected;
}

bool UJWFunctionLibrary::CheckCollisionTrace_NearestOverlapByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutNearestFirstOverlapResults, FName TraceTag, bool bIgnoreSelf)
{
    bool bHitDetected = CheckCollisionTrace_OverlapMultiByChannel(SourceActor, Collision, TraceChannel, OutNearestFirstOverlapResults, TraceTag,bIgnoreSelf);

    const FVector SourceLocation = SourceActor->GetActorLocation();


    // 비용이 발생할 수 있지만, 비용을 비교해서 사용하자. 
    OutNearestFirstOverlapResults.Sort([&SourceLocation](const FOverlapResult& A, const FOverlapResult& B)
        {
            float DistA = FVector::DistSquared(SourceLocation, A.GetActor()->GetActorLocation());
            float DistB = FVector::DistSquared(SourceLocation, B.GetActor()->GetActorLocation());
            return DistA < DistB; // 오름차순 (가까운 순)
        });


    return bHitDetected;
}

bool UJWFunctionLibrary::CheckCollisionTrace_LineSingleByChannel(AActor* SourceActor, const FVector Start, const FVector End, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag, bool bIgnoreSelf)
{
    // 충돌 파라미터
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);

    // 라인 트레이스 실행
    bool bHit = SourceActor->GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        Start,
        End,
        TraceChannel,
        Params
    );

    return bHit;
}

bool UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(AActor* SourceActor, const FVector Start, const FVector End, FCollisionShape Collision, ECollisionChannel TraceChannel, FHitResult& OutHitResult, FName TraceTag, bool bIgnoreSelf)
{   
    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(SourceActor->GetRootComponent());
    if (Capsule != nullptr)
    {
        float CapsuleRadius = Capsule->GetScaledCapsuleRadius();
        FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);
		FVector _Start = Start + (SourceActor->GetActorForwardVector() * CapsuleRadius);
        bool HitDetected = SourceActor->GetWorld()->SweepSingleByChannel(OutHitResult, _Start, End, FQuat::Identity, TraceChannel, Collision, Params);
        return HitDetected;
    }
    return false;
}

bool UJWFunctionLibrary::CheckCollisionTrace_SweepMultiByChannel(AActor* SourceActor, const FVector Start, const FVector End, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FHitResult>& OutHitResults, FName TraceTag, bool bIgnoreSelf)
{
    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(SourceActor->GetRootComponent());
    if (Capsule != nullptr)
    {
        float CapsuleRadius = Capsule->GetScaledCapsuleRadius();
        FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);
        FVector _Start = Start + (SourceActor->GetActorForwardVector() * CapsuleRadius);
        bool HitDetected = SourceActor->GetWorld()->SweepMultiByChannel(OutHitResults, _Start, End, FQuat::Identity, TraceChannel, Collision, Params);
        return HitDetected;
    }
    return false;
}

bool UJWFunctionLibrary::CheckCollisionTrace_OverlapMultiByChannel(AActor* SourceActor, const FVector Start, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutOverlapResults, FName TraceTag, bool bIgnoreSelf)
{
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceTag), false, (bIgnoreSelf) ? SourceActor : nullptr);
 

    bool bHitDetected = SourceActor->GetWorld()->OverlapMultiByChannel(
        OutOverlapResults,
        Start,
        FQuat::Identity,
        TraceChannel,
        Collision,
        Params
    );

    return bHitDetected;
}

bool UJWFunctionLibrary::CheckCollisionTrace_NearestOverlapByChannel(AActor* SourceActor, const FVector Start, FCollisionShape Collision, ECollisionChannel TraceChannel, TArray<FOverlapResult>& OutNearestFirstOverlapResults, FName TraceTag, bool bIgnoreSelf)
{
    bool bHitDetected = CheckCollisionTrace_OverlapMultiByChannel(SourceActor,Start, Collision, TraceChannel, OutNearestFirstOverlapResults, TraceTag, bIgnoreSelf);

    // 비용이 발생할 수 있지만, 비용을 비교해서 사용하자. 
    OutNearestFirstOverlapResults.Sort([&Start](const FOverlapResult& A, const FOverlapResult& B)
        {
            float DistA = FVector::DistSquared(Start, A.GetActor()->GetActorLocation());
            float DistB = FVector::DistSquared(Start, B.GetActor()->GetActorLocation());
            return DistA < DistB; // 오름차순 (가까운 순)
        });


    return bHitDetected;
}

void UJWFunctionLibrary::Debug_ShapeSingleByChannel(AActor* SourceActor, float Range, FCollisionShape Collision, bool HitDetected)
{
    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(SourceActor->GetRootComponent());
    if (Capsule != nullptr)
    {
        float Radius = Capsule->GetScaledCapsuleRadius();
    
        const FVector Forward = SourceActor->GetActorForwardVector();
        const FVector Start = SourceActor->GetActorLocation() + Forward * Radius;
        const FVector End = Start + Forward * Range;

        switch (Collision.ShapeType)
        {
            case ECollisionShape::Sphere:
            {
                FVector SphereOrigin = Start + (End - Start) * 0.5f;
                float SphereRadius = Collision.GetSphereRadius();
                FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

                DrawDebugSphere(
                    SourceActor->GetWorld(),
                    SphereOrigin,
                    SphereRadius,
                    16,                // 세그먼트 수 (디버그용)
                    DrawColor,
                    false,
                    5.0f
                );

                break;
            }
            case ECollisionShape::Box:
            {
                FVector BoxOrigin = Start + (End - Start) * 0.5f;
                FVector BoxExtent = Collision.GetBox();
                FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

                DrawDebugBox(
                    SourceActor->GetWorld(),
                    BoxOrigin,
                    BoxExtent,
                    FRotationMatrix::MakeFromZ(Forward).ToQuat(),
                    DrawColor,
                    false,
                    5.0f
                );

                break;
            }
            case ECollisionShape::Capsule:
            {
                FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
                float CapsuleHalfHeight = Range * 0.5f;
                FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

                DrawDebugCapsule(
                    SourceActor->GetWorld(),
                    CapsuleOrigin,
                    CapsuleHalfHeight,
                    Collision.GetCapsuleRadius(),
                    FRotationMatrix::MakeFromZ(Forward).ToQuat(),
                    DrawColor,
                    false,
                    5.0f
                );
                break;
            }
            default:
            {
                // 여기로 들어오면 잘못 넣은 경우.
                // 로그를 남기자 
                ensure(false);
                break;
            }
        }
    
    
    }
   
}

void UJWFunctionLibrary::Debug_ShapeSingleByChannel(AActor* SourceActor, const FVector Start, const FVector End, FCollisionShape Collision, bool HitDetected)
{
	const FVector Direction = (End - Start).GetSafeNormal();
	const float Range = FVector::Dist(Start, End);

    switch (Collision.ShapeType)
    {
    case ECollisionShape::Sphere:
    {
        FVector SphereOrigin = Start + (End - Start) * 0.5f;
        float SphereRadius = Collision.GetSphereRadius();
        FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

        DrawDebugSphere(
            SourceActor->GetWorld(),
            SphereOrigin,
            SphereRadius,
            16,                // 세그먼트 수 (디버그용)
            DrawColor,
            false,
            5.0f
        );

        break;
    }
    case ECollisionShape::Box:
    {
        FVector BoxOrigin = Start + (End - Start) * 0.5f;
        FVector BoxExtent = Collision.GetBox();
        FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

        DrawDebugBox(
            SourceActor->GetWorld(),
            BoxOrigin,
            BoxExtent,
            FRotationMatrix::MakeFromZ(Direction).ToQuat(),
            DrawColor,
            false,
            5.0f
        );

        break;
    }
    case ECollisionShape::Capsule:
    {
        FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
        float CapsuleHalfHeight = Range * 0.5f;
        FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

        DrawDebugCapsule(
            SourceActor->GetWorld(),
            CapsuleOrigin,
            CapsuleHalfHeight,
            Collision.GetCapsuleRadius(),
            FRotationMatrix::MakeFromZ(Direction).ToQuat(),
            DrawColor,
            false,
            5.0f
        );
        break;
    }
    default:
    {
        // 여기로 들어오면 잘못 넣은 경우.
        // 로그를 남기자 
        ensure(false);
        break;
    }
    }

}

void UJWFunctionLibrary::Debug_Direction(AActor* SourceActor, const FVector Direction, float Length, FColor Color, float Duration)
{
    if (SourceActor != nullptr)
    {
		const FVector Start = SourceActor->GetActorLocation();
		const FVector End = Start + Direction * Length;
        DrawDebugDirectionalArrow(
            SourceActor->GetWorld(),
            Start,              // 화살표 시작 위치
            End,                // 화살표 끝 위치
            50.f,               // 화살표 크기
            Color,              // 색상
            false,              // 지속 여부 (true면 계속 표시)
            Duration,           // 표시 시간 (초)
            10,                 // 깊이 우선순위
            2.0f                // 두께
        );

    }
    
}

FVector UJWFunctionLibrary::ComputePositionFromMouse(APlayerController* PlayerController, ECollisionChannel Channel)
{
    FHitResult Hit;
    if (PlayerController != nullptr)
    {
        AActor* MyActor = PlayerController->GetPawn();
        if (MyActor != nullptr)
        {
            PlayerController->GetHitResultUnderCursor(Channel, false, Hit);
            return Hit.ImpactPoint;
        }
    }
    return FVector::ZeroVector;
}

void UJWFunctionLibrary::ComputeRotationFromMouse(APlayerController* PlayerController, ECollisionChannel Channel)
{
    FHitResult Hit;
    if (PlayerController != nullptr)
    {
        AActor* MyActor = PlayerController->GetPawn();
        if (MyActor != nullptr)
        {
            PlayerController->GetHitResultUnderCursor(Channel, false, Hit);
            if (Hit.bBlockingHit)
            {
                FVector TargetLocation = Hit.Location;

                // 캐릭터 위치와 마우스 위치를 연결해 회전값 계산
                FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(MyActor->GetActorLocation(), TargetLocation);

                // Pitch, Roll은 무시하고 Yaw만 적용
                FRotator NewRotation(0.f, LookAtRotation.Yaw, 0.f);
                MyActor->SetActorRotation(NewRotation);
            }
        }
    }
}

FRotator UJWFunctionLibrary::ComputeLookAtRotationFromMouse(APlayerController* PlayerController, ECollisionChannel Channel)
{
    FHitResult Hit;
    if (PlayerController != nullptr)
    {
        AActor* MyActor = PlayerController->GetPawn();
        if (MyActor != nullptr)
        {
            PlayerController->GetHitResultUnderCursor(Channel, false, Hit);
            if (Hit.bBlockingHit)
            {
                FVector TargetLocation = Hit.Location;

                // 캐릭터 위치와 마우스 위치를 연결해 회전값 계산
                FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(MyActor->GetActorLocation(), TargetLocation);

                // Pitch, Roll은 무시하고 Yaw만 적용
                FRotator NewRotation(0.f, LookAtRotation.Yaw, 0.f);
                return NewRotation;
            }
        }
    }
    return FRotator::ZeroRotator;
}

FVector UJWFunctionLibrary::ComputePositionFromMouse(AActor* SourceActor, ECollisionChannel Channel)
{
    if (SourceActor != nullptr)
    {
        APlayerController* PC = Cast<APlayerController>(CastChecked<APawn>(SourceActor)->GetController());
        return ComputePositionFromMouse(PC, Channel);
    }

    return FVector::ZeroVector;
}

void UJWFunctionLibrary::ComputeRotationFromMouse(AActor* SourceActor, ECollisionChannel Channel)
{
    if (SourceActor != nullptr)
    {
        APlayerController* PC = Cast<APlayerController>(CastChecked<APawn>(SourceActor)->GetController());
        ComputeRotationFromMouse(PC, Channel);
    }

}

FRotator UJWFunctionLibrary::ComputeLookAtRotationFromMouse(AActor* SourceActor, ECollisionChannel Channel)
{
    if (SourceActor != nullptr)
    {
        APlayerController* PC = Cast<APlayerController>(CastChecked<APawn>(SourceActor)->GetController());
	    return ComputeLookAtRotationFromMouse(PC, Channel);
    }

	return FRotator::ZeroRotator;
}

TArray<FString> UJWFunctionLibrary::SplitTagToArray(const FGameplayTag& GameplayTag)
{
    TArray<FString> Result;
    GameplayTag.GetTagName().ToString().ParseIntoArray(Result, TEXT("."));
    return Result;
}

TArray<FString> UJWFunctionLibrary::GetTagPrefixArray(const FGameplayTag& GameplayTag, uint32 Index)
{
    TArray<FString> Result;
    GameplayTag.GetTagName().ToString().ParseIntoArray(Result, TEXT("."));
    for (uint32 i = Result.Num(); i >= Index; i--)
    {
        Result.Pop();
    }
    return Result;
}

FString UJWFunctionLibrary::GetTagSegmentAt(const FGameplayTag& GameplayTag, uint32 Index)
{
    TArray<FString> StrArray;
    GameplayTag.GetTagName().ToString().ParseIntoArray(StrArray, TEXT("."));
    FString Result;
    for (; Index < uint32(StrArray.Num()); Index++)
    {
        Result += StrArray[Index];
    }
    return Result;
}

FString UJWFunctionLibrary::GetTagSuffixString(const FGameplayTag& GameplayTag, uint32 Index)
{

    TArray<FString> Result;
    GameplayTag.GetTagName().ToString().ParseIntoArray(Result, TEXT("."));
    if (Result.IsValidIndex(Index))
    {
        return Result[Index];
    }
    else
    {
        return FString();
    }
}






