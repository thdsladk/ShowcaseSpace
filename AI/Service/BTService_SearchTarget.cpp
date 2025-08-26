// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SearchTarget.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
//#include "GameFramework/Pawn.h"

#include "Interface/BehaviorInterface.h"
#include "Interface/TargetInterface.h"

#include "Algo/Sort.h"

constexpr uint8 FirstIndex = 0U;

UBTService_SearchTarget::UBTService_SearchTarget()
	:m_IsDebug(false)
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
	ITargetInterface* TI = Cast<ITargetInterface>(CurrentPawn);
	if (TI == nullptr)
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
		ECollisionChannel::ECC_GameTraceChannel4,			// 4���� Attack_Enermy �浹�� 
		FCollisionShape::MakeSphere(SearchRadius),			// ������ �浹 ����̴�.
		QueryParams);

	enum ERadiusType :uint8{Melee,Ranged,Detection,Visible,Outside};
	if (bResult == true)
	{
		TArray<AActor*> Targets;
		// �ߺ� ����
		for (auto& OverlapResult : OverlapResults)
		{
			Targets.AddUnique(OverlapResult.GetActor());
		}
		// �Ÿ� ������ Ÿ�� �����ϱ� // Algo::Sort�� TArray::Sort���� �γ��ϴ�.
		Algo::Sort(Targets,
			[&CurrentPawn](const AActor* A, const AActor* B)
			{
				return FMath::Abs(CurrentPawn->GetDistanceTo(A)) < FMath::Abs(CurrentPawn->GetDistanceTo(B));
			});

		
		// ĳ���Ϳ� Ÿ�� �߰�
		TI->AddTargets(Targets);

		// ���� ����� Ÿ�� ����
		APawn* TargetPawn = CastChecked<APawn>(Targets[FirstIndex]);
		if (TargetPawn != nullptr && TargetPawn->GetController()->IsPlayerController() == true)
		{

			double Dist = FMath::Abs((Center - TargetPawn->GetActorLocation()).Size());		/* ���� ������ ���� ����� ��� ... */ /* �׷���.. double�� ���� �ȳ��ͼ� �ٽ� ... ��� */
			//double Dist = Target->GetDistanceTo(CurrentPawn);							/* ���� �Լ��� ����ϴ°� ������ ... */
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

			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::RadiusTypeKey, RadiusType);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, TargetPawn);

			if (m_IsDebug == true)
			{
				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
				UE_LOG(LogTemp, Warning, TEXT("Target is Target  :: Count %d"), ++m_CountDebug);
			}

			return;
		}


	}
	else
	{
		TI->ClearTargets();
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::RadiusTypeKey, ERadiusType::Outside);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr);
		
		if (m_IsDebug == true)
		{
			DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
			UE_LOG(LogTemp, Warning, TEXT("Target is Empty  :: Count %d"), ++m_CountDebug);
		}
	}
}