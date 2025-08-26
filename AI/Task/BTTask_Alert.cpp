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

	// �ʱ� �� ���õ�
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
	//	// Ư�� ��(��: "Head")�� ���� ��ġ�� ����ϴ�.
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


	// �տ� ������ ���������� ���ؼ� ������ �������� �帣���� ����
	FVector LookVector = (Target->GetActorLocation() - Self->GetActorLocation());
	double Distance = LookVector.Size();
	LookVector.Normalize();


	//ȸ���� ���� ���
	FVector Direction_Left = LookVector.RotateAngleAxis(-90.0f, FVector::UpVector);
	FVector Direction_Right= LookVector.RotateAngleAxis(90.0f, FVector::UpVector);

	// �� ���� ������ ���� ��� (LeftDirection�� LookVector, RightDirection�� LookVector ��� ���) 
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


	// 1) �ʱ�ȭ: �Ÿ��� ���� ���� ����
	FVector ToSelf = Self->GetActorLocation() - Target->GetActorLocation();
	ToSelf.Z = 0.f;
	ToSelf.Normalize();

	// 2) ���� ������Ʈ (Radians)
	float DirMul = (m_Direction == 0) ? TurnRight : TurnLeft;
	if (m_Radius <= 0.0) m_Radius = 1.0;
	// �׳� �ð� �� ��ӿ���� ȸ�� �ӵ��� ������ ������.
	//float DeltaRad = DirMul * ((m_Speed * DeltaSeconds) / m_Radius);
	float DeltaRad = DirMul * 0.1f;
	
	// Atan2(Y,X)�� �̿��� �ʱ� ���� ���� ���
	m_AngleRad = FMath::Atan2(ToSelf.Y, ToSelf.X) + DeltaRad;
	// ~PI ���� PI�� �� ����
	m_AngleRad = FMath::UnwindRadians(m_AngleRad);


	// 3) ���ο� ������ �� �̵�
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

