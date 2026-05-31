// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/GASAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceBase.h"

#include "Header/JWHeader.h"


AGASAIController::AGASAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{

	m_bEnableDetourCrowdAvoidance = true;
	m_DetourCrowdAvoidanceQuality = 4;
	m_CollisionQueryRange = 600.f;
	
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 700.f;
	AISenseConfig_Sight->LoseSightRadius = 800.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 80.f;

	m_EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	m_EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	m_EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	m_EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));

	


	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/JWGAS/Blueprint/AI/BlackBoard/BB_NPCBase.BB_NPCBase"));
	//if (BBAssetRef.Object != nullptr)
	//{
	//	m_BBAsset = BBAssetRef.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/JWGAS/Blueprint/AI/BehaviorTree/BT_NPC.BT_NPC"));
	//if (BTAssetRef.Object != nullptr)
	//{
	//	m_BTAsset = BTAssetRef.Object;
	//
	//}
}

ETeamAttitude::Type AGASAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AGASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(m_bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		
		switch (m_DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default:
			break;
		}
		
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(m_CollisionQueryRange);
	}
}

void AGASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();

	// Setting MaxWalkSpeed
	UCharacterMovementComponent* MovementComp = CastChecked<ACharacter>(InPawn)->GetCharacterMovement();
	GetBlackboardComponent()->SetValueAsFloat(FName("DefaultMaxWalkSpeed"), MovementComp->MaxWalkSpeed);

}

void AGASAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (BlackboardComponent->GetValueAsObject(FName("TargetActor")) == nullptr)
		{
			if (Stimulus.WasSuccessfullySensed() == true && Actor != nullptr)
			{
				
				//Debug::Print(Actor->GetActorNameOrLabel() + TEXT(" was sensed"), FColor::Green);
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
				//UE_LOG(LogTemp, Warning, TEXT("TargetActor: %s"), *GetNameSafe(GetBlackboardComponent()->GetValueAsObject("TargetActor")));
				BlackboardComponent->SetValueAsVector(FName("TargetPos"), Actor->GetActorLocation());
			}
		}
		else
		{
			if (Stimulus.WasSuccessfullySensed() == false)
			{
				BlackboardComponent->ClearValue(FName("TargetActor"));
				BlackboardComponent->ClearValue(FName("TargetPos"));
			}
		}
	}

	ComputeViewAngle(Actor);
}

void AGASAIController::RunAI()
{
	UBlackboardComponent* BBComp = nullptr;

	if (UseBlackboard(m_BBAsset, BBComp))
	{
		Blackboard = BBComp;
		bool RunResult = RunBehaviorTree(m_BTAsset);
		ensure(RunResult);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("BrainComponent: %s"), *GetNameSafe(BrainComponent));

}

void AGASAIController::StopAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void AGASAIController::ComputeViewAngle(const AActor* Actor)
{
	ACharacter* OwnerCharacter = GetCharacter();
	if (OwnerCharacter != nullptr)
	{
		if (UAnimInstanceBase* AnimInst = Cast<UAnimInstanceBase>(OwnerCharacter->GetMesh()->GetAnimInstance()))
		{
			FVector Forward = OwnerCharacter->GetActorForwardVector();
			FVector ToTarget = (Actor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();

			// Dot으로 각도 크기
			float AngleRad = FMath::Acos(FVector::DotProduct(Forward, ToTarget));
			float AngleDeg = FMath::RadiansToDegrees(AngleRad);

			// Cross로 방향 판별
			FVector Cross = FVector::CrossProduct(Forward, ToTarget);

			if (Cross.Z < 0)
			{
				AngleDeg = -AngleDeg;
			}

			AnimInst->SetViewAngle(AngleDeg);
		}
	}


}
