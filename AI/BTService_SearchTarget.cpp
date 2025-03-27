// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
//#include "GameFramework/Pawn.h"

#include "Interface/BehaviorInterface.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SerchTarget");
	Interval = 1.0f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(CurrentPawn);
	if (BI == nullptr)
		return;


	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = BI->GetVisibleRadius();				

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults{};
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,			// 14번은 Attack_Enermy 충돌로 
		FCollisionShape::MakeSphere(SearchRadius),			// 적들의 충돌 대상이다.
		QueryParams);

	enum ERadiusType :uint8{Melee,Ranged,Detection,Visible,Outside};
	if (bResult == true)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			APawn* Target = Cast<APawn>(OverlapResult.GetActor());
			
			if (Target != nullptr && Target->GetController()->IsPlayerController())
			{

				double Dist = FMath::Abs((Center - Target->GetActorLocation()).Size());		/* 절대 값으로 직접 계산한 방식 ... */ /* 그러나.. double로 값이 안나와서 다시 ... 계산 */
				//double Dist = Target->GetDistanceTo(CurrentPawn);							/* 내장 함수로 계산하는게 나을듯 ... */
				ERadiusType RadiusType = ERadiusType::Outside;
				
				if (Dist < BI->GetMeleeRadius())
				{
					RadiusType = ERadiusType::Melee;
				}
				else if (Dist < BI->GetRangedRadius())
				{
					RadiusType = ERadiusType::Ranged;
				}
				else if (Dist < BI->GetDetectionRadius())
				{
					RadiusType = ERadiusType::Detection;
				}
				else
				{
					RadiusType = ERadiusType::Visible;
				}
				
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("RadiusType")), RadiusType);
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Target);


				//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f); 
				//UE_LOG(LogTemp, Warning, TEXT("Target is Target  :: Count %d"), ++Debug);

				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("RadiusType")), ERadiusType::Outside);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("RadiusType")), ERadiusType::Outside);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	
		//UE_LOG(LogTemp, Warning, TEXT("Target is Empty  :: Count %d"), ++Debug);
	}
}