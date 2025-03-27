// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"		// UAISenseConfig_Sight
#include "Perception/AISenseConfig_Hearing.h"	// UAISenseConfig_Hearing
#include "Perception/AISenseConfig_Damage.h"	// UAISenseConfig_Damage

#include "Header/GlobalEnum.h"

// 캐릭터가 플레이어 캐릭터인지 알기 위해서 
#include "Character/CharacterBase.h"

const FName AMyAIController::HomePosKey(TEXT("HomePos"));
const FName AMyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMyAIController::TargetKey(TEXT("Target"));
const FName AMyAIController::StateKey(TEXT("State"));
const FName AMyAIController::BattleCommandKey(TEXT("BattleCommand"));
const FName AMyAIController::NPCModeKey(TEXT("NPCMode"));
const FName AMyAIController::DestPosKey(TEXT("DestPos"));
const FName AMyAIController::HasDestPosKey(TEXT("HasDestPos"));


AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/TopDown/AI/BT_NPC.BT_NPC"));
	//check(BT.Succeeded())
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Game/TopDown/AI/BB_NPC.BB_NPC"));
	//check(BD.Succeeded())
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}

	m_AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*m_AIPerception);

	// Sight Config
	m_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	m_SightConfig->SightRadius = 800.f;
	m_SightConfig->LoseSightRadius = 1200.f;
	m_SightConfig->PeripheralVisionAngleDegrees = 60.f;				// 60도의 시야각 (좌,우 합 120도)
	m_SightConfig->SetMaxAge(5.f);									// 5초간 유지
	m_SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	// Detect only specific actors 
	m_SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	m_SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	m_AIPerception->ConfigureSense(*m_SightConfig);
	m_AIPerception->SetDominantSense(m_SightConfig->GetSenseImplementation());
	
	m_HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	m_HearingConfig->HearingRange = 500.f;
	m_HearingConfig->SetMaxAge(3.f);
	
	m_HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	m_HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	m_AIPerception->ConfigureSense(*m_HearingConfig);
	// ...

	m_AIPerception->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::PerceptionUpdated);

}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyAIController::RandomMove, 3.f, true);
	
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BlackboardData, BlackboardComp))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());	// 블랙보드에 HomePosKey를 대입 

		Blackboard->SetValueAsEnum(StateKey, 0);								// State 값 0으로 초기화 
		if (RunBehaviorTree(BehaviorTree))
		{
			// TODO
		}
	}
	
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AMyAIController::RunAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::Looped);
	}
}

void AMyAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

bool AMyAIController::AddCommandQueue(uint8 Command)
{
	return m_CommandQueue.Enqueue(Command);
}

bool AMyAIController::UseCommandQueue(uint8& Command)
{
	return m_CommandQueue.Dequeue(Command);
}

bool AMyAIController::IsEmptyCommandQueue()
{
	return m_CommandQueue.IsEmpty();
}

void AMyAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		FAIStimulus AIStimulus;
		AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_Sight);
		if (AIStimulus.WasSuccessfullySensed())
		{
			HandleSensedSight(UpdatedActor);
		}
		AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_Hearing);
		if (AIStimulus.WasSuccessfullySensed())
		{
			HandleSensedHearing(UpdatedActor,AIStimulus.StimulusLocation);
		}
		AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_Damage);
		if (AIStimulus.WasSuccessfullySensed())
		{
			HandleSensedDamage(UpdatedActor);
		}
	}
}

FAIStimulus AMyAIController::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	m_AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case EAIPerceptionSense::EPS_None:
		break;
	case EAIPerceptionSense::EPS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Hearing:
		QuerySenseClass = UAISense_Hearing::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	case EAIPerceptionSense::EPS_MAX:
		break;
	default:
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass;

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);


		if (QuerySenseClass == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;
			return ResultStimulus;
		}

	}
	return ResultStimulus;
}

void AMyAIController::HandleSensedSight(AActor* Actor)
{
	// 여기서 시야각에 들어 왔을 때 상태를 변환한다. 

	// 공격 명령을 해도 전투모드가 되지는 않는다 .
	//Blackboard->SetValueAsEnum(BattleCommandKey, static_cast<uint8>(EBattleCommand::Attack));
	TWeakObjectPtr<ACharacterBase> Character = CastChecked<ACharacterBase>(Actor);
	if ( Character->IsPlayerCharacter() == true)
	{
		Blackboard->SetValueAsEnum(NPCModeKey, static_cast<uint8>(ECharacterMode::Battle));
	}
}

void AMyAIController::HandleSensedHearing(AActor* Actor,FVector NoiseLocation)
{
	// 여기서 소리를 인지했을때 상태를 변환한다. 
	

	TWeakObjectPtr<ACharacterBase> Character = CastChecked<ACharacterBase>(Actor);

	if (Character->IsPlayerCharacter() == true)
	{
		Blackboard->SetValueAsEnum(NPCModeKey, static_cast<uint8>(ECharacterMode::Battle));
		Blackboard->SetValueAsObject(TargetKey, Actor);
		Blackboard->SetValueAsVector(DestPosKey, NoiseLocation);
		Blackboard->SetValueAsBool(HasDestPosKey, true);
	}
}

void AMyAIController::HandleSensedDamage(AActor* Actor)
{

}

void AMyAIController::RandomMove()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;
	
	FNavLocation RandomLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
	}
}
