// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Service/BTService_Decision.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Interface/BehaviorInterface.h"
#include "Data/MyGameSubsystem.h"

UBTService_Decision::UBTService_Decision()
{
	NodeName = TEXT("Service_Decision");
	Interval = 1.f;



	/*
	
	���� ���� �Ÿ� , ���� ���� �Ÿ�, �ν� �Ÿ� 
	
	Attack / SmashAttck 
	: ���� ������ ���� ���� �������� �̵��ϰ� ���� 
	/ Defense 
	/ Cast 
	: � �����̳Ŀ� ���� �޶��� 
	/ FallBack
	: ���� ��ġ�� ���� 
	/ Shout 
	: Ư�� ��Ȳ���� ���� �θ��� ( ���� �ൿ ���Ϳ��� ������ Ư¡ ) 
	/ Escape
	: ���� 

	
	���� �Ÿ� 
	�񰡽� �Ÿ� 
	

	*/

	/*
	
	Null			
	Attack			
	SmashAttack			
	Defense			
	Cast
	Chase
	FallBack		// �ڷ� ��� ���� 			
	Shout			
	Escape			
	End				'
	
	*/

	/*
		TQueue
		Peek() ��ȯ
		Pop()  First In ���� 
		IsEmpty() ����ִ��� ?
		Enqueue() �߰�
		Dequeue() ���ڷ� ���� ��ȯ�ϰ� ���ŵ� ���� ť�� ��������� false ť�� ���������� true
	*/
}

void UBTService_Decision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
uint8 RadiusType = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AMyAIController::RadiusTypeKey);
uint8 CurrentCommand = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AMyAIController::BattleCommandKey);

// �ֱ������� �����ϴ� ����
IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
// ��׷� ������ �ڿ����.
//BI->SetAggroGauge(BI->GetAggroGauge() + 1);

if (static_cast<uint8>(ERadiusType::Visible) < RadiusType)
{
	// �νİŸ��� ����� �ൿ�� ���� �ʴ´�. 
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, NULL);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::NPCModeKey, NULL);
	}
	else
	{
		// �̵� ������ ���ش�. 
		UMyGameSubsystem* GameSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UMyGameSubsystem>();
		if (GameSubsystem != nullptr)
		{
			const FCommandData* CommandData = GameSubsystem->GetCommandData(static_cast<uint8>(CurrentCommand));
			if (CommandData != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::HasDestPosKey, CalculateDestination(OwnerComp, CommandData->RadiusType));
			}
		}	

	}

	return;

}

bool UBTService_Decision::CalculateDestination(UBehaviorTreeComponent& OwnerComp, ERadiusType DestRadiusType)
{
	/*
		��ǥ �ݰ��� �߰� ��ġ�� ����ؼ�, ��ǥ�� �������ִ� �Լ�   ��ȯ���� DestPos�� �������ִ��� ������ ��ȯ
		1. DestRadiusType�� �߰� ���� ���ϱ�
		2. �̵��ؾ��� ���� ���ϱ�  -(�߰����� - Ÿ����ġ)    : -�� �ٴ� ������ ������ �ݴ뿩���ϴϱ�.   ( Ÿ����ġ - �߰����� ) ���� �ص� �ɵ��ϴ� .
		3. �ڽſ��� �̵��� ���͸� �����ָ� ��
	*/

	// End�� �����ŷ� ����ؼ� ����� ���Ѵ�. 
	if (DestRadiusType == ERadiusType::End)
	{
		return false;
	}

	ERadiusType CurRadiusType = static_cast<ERadiusType>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AMyAIController::RadiusTypeKey));
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

	if (DestPos.Equals(Target->GetActorLocation()) == true)
	{
		return false;
	}
	// �����忡 DestPos ����
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::DestPosKey, DestPos);

	// Debug
	//UE_LOG(LogTemp, Log, TEXT("SelfPos : %f,%f,%f "),VecSelf.X, VecSelf.Y, VecSelf.Z);
	//UE_LOG(LogTemp, Log, TEXT("TargetPos : %f,%f,%f "),VecTarget.X, VecTarget.Y, VecTarget.Z);
	//UE_LOG(LogTemp, Log, TEXT("DestPos : %f,%f,%f "),DestPos.X, DestPos.Y, DestPos.Z);


	return true;
}
