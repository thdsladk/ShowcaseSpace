// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Alert.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/BehaviorInterface.h"

constexpr float TurnLeft = -1.f;
constexpr float TurnRight = 1.f;

UBTTask_Alert::UBTTask_Alert()
{
	NodeName = TEXT("Alert");
	bNotifyTick = true;
	bCreateNodeInstance = true;
	
	m_RandomStream.Initialize(GetUniqueID() ^ FDateTime::Now().GetMillisecond());
	m_Direction = FMath::RandBool();

	m_IsPlaying = false;
}

EBTNodeResult::Type UBTTask_Alert::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (m_IsPlaying == true)
		return EBTNodeResult::InProgress;

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* Self = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;
	
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(Self);
	if (nullptr == BI)
		return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	// 초기 값 세팅들
	m_IsPlaying = true;
	m_Radius = BI->GetDetectionRadius();
	BI->LookDirection((m_Direction != 0) ? TurnRight : TurnLeft);

	BI->OnAlert();
	FOnBehaviorAlertEnd Delegate;

	Delegate.AddLambda([this,&OwnerComp]()
		{
			auto _BI = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
			if (nullptr == _BI)
				return;
			this->m_IsPlaying = false;
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, static_cast<uint8>(_BI->GetState()));
			this->FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	BI->SetAlertDelegate(Delegate);

#pragma region DEBUG

	//FVector HeadLocation{};
	//
	//USkeletalMeshComponent* SkeletalMesh = Self->FindComponentByClass<USkeletalMeshComponent>();
	//if (SkeletalMesh != nullptr)
	//{
	//	// 특정 본(예: "Head")의 월드 위치를 얻습니다.
	//	HeadLocation = SkeletalMesh->GetSocketLocation(TEXT("OverHead"));
	//
	//	UE_LOG(LogTemp, Warning, TEXT("Head Bone Location: %s"), *HeadLocation.ToString());
	//}
	//FVector StartPos = HeadLocation;
	//FVector EndPos = HeadLocation+(Self->GetActorForwardVector() * 120.f);
	//DrawDebugDirectionalArrow(GetWorld(), StartPos,EndPos,240.f, FColor::Magenta, false, 3.f, 0, 3.f);

#pragma endregion
	
	return EBTNodeResult::InProgress;
}
/*
void UBTTask_Alert::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Self = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return;
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(Self);
	if (nullptr == BI)
		return;
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return;


	// 앞에 방향을 더해줌으로 인해서 랜덤한 방향으로 흐르도록 제어
	FVector LookVector = (Target->GetActorLocation() - Self->GetActorLocation());
	double Distance = LookVector.Size();
	LookVector.Normalize();


	//회전된 방향 계산
	FVector Direction_Left = LookVector.RotateAngleAxis(-90.0f, FVector::UpVector);
	FVector Direction_Right= LookVector.RotateAngleAxis(90.0f, FVector::UpVector);

	// 두 벡터 사이의 각도 계산 (LeftDirection과 LookVector, RightDirection과 LookVector 모두 계산) 
	float AngleLeft = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookVector, Direction_Left)));
	float AngleRight = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookVector, Direction_Right)));


	//Debug
	//UE_LOG(LogTemp, Log, TEXT("Left Degree : %d "), AngleLeft);
	//UE_LOG(LogTemp, Log, TEXT("Right Degree : %d "), AngleRight);

	//FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	if (m_Direction == 0)
	{
		BI->LookDirection(1.f);
		OwnerComp.GetAIOwner()->MoveToLocation(Self->GetActorLocation() + (Direction_Left * Distance));
	}
	else
	{
		BI->LookDirection(-1.f);
		OwnerComp.GetAIOwner()->MoveToLocation(Self->GetActorLocation() + (Direction_Right * Distance));
	}																	   






}*/

void UBTTask_Alert::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Self = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return;
	IBehaviorInterface* BI = Cast<IBehaviorInterface>(Self);
	if (nullptr == BI)
		return;
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return;


	// 1) 초기화: 거리와 시작 각도 저장
	FVector ToSelf = Self->GetActorLocation() - Target->GetActorLocation();
	ToSelf.Z = 0.f;
	ToSelf.Normalize();

	// 2) 각도 업데이트 (Radians)
	float DirMul = (m_Direction == 0) ? TurnRight : TurnLeft;
	if (m_Radius <= 0.0) m_Radius = 1.0;
	// 그냥 시간 당 등속운동으로 회전 속도를 고정해 버리자.
	//float DeltaRad = DirMul * ((m_Speed * DeltaSeconds) / m_Radius);
	float DeltaRad = DirMul * 0.1f;
	
	// Atan2(Y,X)를 이용해 초기 라디안 각도 계산
	m_AngleRad = FMath::Atan2(ToSelf.Y, ToSelf.X) + DeltaRad;
	// ~PI 에서 PI로 값 보정
	m_AngleRad = FMath::UnwindRadians(m_AngleRad);


	// 3) 새로운 오프셋 및 이동
	const FVector Offset = FVector(
		FMath::Cos(m_AngleRad),
		FMath::Sin(m_AngleRad),
		0.f
	) * m_Radius;

	const FVector Destination = Target->GetActorLocation() + Offset;
	OwnerComp.GetAIOwner()->MoveToLocation(Destination);
	
	// DEBUG
	//DrawDebugDirectionalArrow(GetWorld(), Self->GetActorLocation(), Destination,120.f, FColor::Magenta,false,1.f,3.f);


	
}

