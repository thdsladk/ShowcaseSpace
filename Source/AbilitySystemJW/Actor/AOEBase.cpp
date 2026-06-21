// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AOEBase.h"
#include "Interface/PlayerControllerInterface.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"



AAOEBase::AAOEBase()
	:m_PlayerController(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAOEBase::BeginPlay()
{
	Super::BeginPlay();
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn != nullptr)
	{
		AController* Controller = Pawn->GetController();
		if (Controller != nullptr)
		{
			m_PlayerController = CastChecked<APlayerController>(Controller);
			//IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Controller);
			//m_ChangeMouseMoveHandle = PCI->OnChangeMouseMove().AddUObject(this, &AAOEBase::ChangeMouseMove);
		}
	}
}

void AAOEBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(m_PlayerController);
	if (PCI != nullptr && PCI->OnChangeMouseMove().IsBoundToObject(this) == true)
	{
		PCI->OnChangeMouseMove().Remove(m_ChangeMouseMoveHandle);
	}
}

void AAOEBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMouseTracking == true)
	{
		if (m_PlayerController != nullptr)
		{
			FHitResult MousePoint;
			bool bHit = m_PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MousePoint);
			if (bHit == true)
			{
				SetActorLocation(MousePoint.ImpactPoint);
				// 데칼이 범위 안에 있는지 체크해서 색깔 바꿔주기
				if (m_ScopeRange > 0.f)
				{
					float Distance = FVector::Distance(MousePoint.ImpactPoint, GetOwner()->GetActorLocation());
					UDecalComponent& DecalComponent = *GetDecal();
					if (Distance > m_ScopeRange)
					{
						SetDecalColor_Red();
					}
					else
					{
						SetDecalColor_Green();
					}

				}
			}

		}
	}
	else
	{
		SetActorLocation(GetOwner()->GetActorLocation());
	}



	
}

uint8 AAOEBase::GetDecalColor()
{
	return m_CurrentColorIndex;
}

void AAOEBase::SetDecalColor_Red()
{
	FString MPCLoad = TEXT("/Game/JWGAS/Material/AOE/MPC_Decal_AOE.MPC_Decal_AOE");
	UMaterialParameterCollection* MPC = LoadObject<UMaterialParameterCollection>(nullptr, *MPCLoad);
	UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(MPC);

	if (MPCInstance != nullptr)
	{
		FName MPCRedVector = MPC->VectorParameters[0].ParameterName;
		FName MPCGreenVector = MPC->VectorParameters[1].ParameterName;
		FLinearColor ChangeColor;
		if (MPCInstance->GetVectorParameterValue(MPCGreenVector, ChangeColor) == true)
		{
			m_CurrentColorIndex = 1U;
			MPCInstance->SetVectorParameterValue(MPCRedVector, FLinearColor::Red);
		}
	}
}

void AAOEBase::SetDecalColor_Green()
{
	FString MPCLoad = TEXT("/Game/JWGAS/Material/AOE/MPC_Decal_AOE.MPC_Decal_AOE");
	UMaterialParameterCollection* MPC = LoadObject<UMaterialParameterCollection>(nullptr, *MPCLoad);
	UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(MPC);

	if (MPCInstance != nullptr)
	{
		FName MPCRedVector = MPC->VectorParameters[0].ParameterName;
		FName MPCGreenVector = MPC->VectorParameters[1].ParameterName;
		FLinearColor ChangeColor;
		if (MPCInstance->GetVectorParameterValue(MPCRedVector, ChangeColor) == true)
		{
			m_CurrentColorIndex = 0U;
			MPCInstance->SetVectorParameterValue(MPCRedVector, FLinearColor::Green);
		}
	}
}

void AAOEBase::ChangeMouseMove()
{
	if (IsMouseTracking == true)
	{
		if (m_PlayerController != nullptr)
		{
			FHitResult MousePoint;
			bool bHit = m_PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MousePoint);
			if (bHit == true)
			{
				SetActorLocation(MousePoint.ImpactPoint);
				// 데칼이 범위 안에 있는지 체크해서 색깔 바꿔주기
				if (m_ScopeRange > 0.f)
				{
					float Distance = FVector::Distance(MousePoint.ImpactPoint, GetOwner()->GetActorLocation());
					if (Distance > m_ScopeRange)
					{
						GetDecal()->DecalColor = FLinearColor::Red;
					}
					else
					{
						GetDecal()->DecalColor = FLinearColor::Green;
					}

				}
			}
		}
	}
	else
	{
		SetActorLocation(GetOwner()->GetActorLocation());
	}
}
