// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Decision.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Interface/BehaviorInterface.h"

UBTService_Decision::UBTService_Decision()
{
	NodeName = TEXT("Service_Decision");
	Interval = 1.f;



	/*
	
	근접 공격 거리 , 원격 공격 거리, 인식 거리 
	
	Attack / SmashAttck 
	: 공격 종류에 따라서 근접 원격으로 이동하고 공격 
	/ Defense 
	/ Cast 
	: 어떤 마법이냐에 따라서 달라짐 
	/ FallBack
	: 원격 위치로 후퇴 
	/ Shout 
	: 특수 상황에서 동료 부르기 ( 집단 행동 몬스터에게 가지는 특징 ) 
	/ Escape
	: 도주 

	
	가시 거리 
	비가시 거리 
	

	*/

	/*
	
	Null			
	Attack			
	SmashAttack			
	Defense			
	Cast
	Chase
	FallBack		// 뒤로 잠시 후퇴 			
	Shout			
	Escape			
	End				'
	
	*/

	/*
		TQueue
		Peek() 반환
		Pop()  First In 제거 
		IsEmpty() 비어있는지 ?
		Enqueue() 추가
		Dequeue() 인자로 값을 반환하고 제거도 진행 큐가 비어있으면 false 큐가 남아있으면 true
	*/
}

void UBTService_Decision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
uint8 RadiusType = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("RadiusType"));
uint8 CurrentCommand = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("BattleCommand"));
	/*
	* 5초에 한번씩 전투명령을 리셋하자 . 
		전투 명령에 대한 판단을 내리자 

		공격 / 강공격
		방어 
		시전
		재정비 [후퇴]
		소리치다
		도주

	*/
	

	if(static_cast<uint8>(ERadiusType::Visible)< RadiusType )
	{
		// 인식거리를 벗어나면 행동을 하지 않는다. 
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), NULL);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NPCMode"), NULL);
	}
	else
	{

		// 이동 세팅을 해준다. 
		FCommandData* CommandData = CastChecked<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetCommandData(static_cast<uint8>(CurrentCommand));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("HasDestPos"), CalculateDestination(OwnerComp, CommandData->RadiusType));

	}

	return;

}

bool UBTService_Decision::CalculateDestination(UBehaviorTreeComponent& OwnerComp, ERadiusType DestRadiusType)
{
	/*
		목표 반경의 중간 위치를 계산해서, 목표로 세팅해주는 함수   반환값은 DestPos를 가지고있는지 없는지 반환
		1. DestRadiusType의 중간 지점 구하기
		2. 이동해야할 벡터 구하기  -(중간지점 - 타겟위치)    : -가 붙는 이유는 방향이 반대여야하니까.   ( 타겟위치 - 중간지점 ) 으로 해도 될듯하다 .
		3. 자신에게 이동할 벡터를 더해주면 끝
	*/

	// End는 같은거로 취급해서 계산을 안한다. 
	if (DestRadiusType == ERadiusType::End)
	{
		return false;
	}

	ERadiusType CurRadiusType = static_cast<ERadiusType>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("RadiusType")));
	if (DestRadiusType == CurRadiusType)
		return false;

	APawn* Self = CastChecked<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	APawn* Target = CastChecked<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(Self);


	FVector VecSelf = Self->GetActorLocation();
	FVector VecTarget = Target->GetActorLocation();
	FVector Direction = (VecTarget - VecSelf).GetSafeNormal();

	float MeleeRadius = BI->GetMeleeRadius();
	float RangedRadius = BI->GetRangedRadius();
	float DetectionRadius = BI->GetDetectionRadius();
	float VisibleRadius = BI->GetVisibleRadius();

	float RadiusMidPosList[static_cast<int32>(ERadiusType::End) + 1]
	{
		(MeleeRadius * 0.5f),
		((MeleeRadius + RangedRadius) * 0.5f),
		((RangedRadius + DetectionRadius) * 0.5f),
		((DetectionRadius + VisibleRadius) * 0.5f),
		0.0f,
	};

	FVector DestPos = (VecTarget - (Direction * RadiusMidPosList[static_cast<int32>(DestRadiusType)]));

	// 블랙보드에 DestPos 세팅
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("DestPos"), DestPos);

	// Debug
	//UE_LOG(LogTemp, Log, TEXT("SelfPos : %f,%f,%f "),VecSelf.X, VecSelf.Y, VecSelf.Z);
	//UE_LOG(LogTemp, Log, TEXT("TargetPos : %f,%f,%f "),VecTarget.X, VecTarget.Y, VecTarget.Z);
	//UE_LOG(LogTemp, Log, TEXT("DestPos : %f,%f,%f "),DestPos.X, DestPos.Y, DestPos.Z);


	return true;
}
