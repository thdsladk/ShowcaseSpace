// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/CharacterBase.h"
#include "Animation/AnimInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Character/MyTest_TopDownPlayerController.h"
#include "Data/MyGameSubsystem.h"

// Header Section
#include "Header/GlobalEnum.h"

// Component Section
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterComponent/MyInventoryComponent.h"
#include "CharacterComponent/MyEquipmentComponent.h"
#include "CharacterComponent/MyStatComponent.h"
#include "CharacterComponent/SkillComponent.h"
#include "CharacterComponent\AbilityComponent.h"
#include "Components/AudioComponent.h"

// Audio Section
#include "Sound/SoundCue.h"

// etc Section
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"

#pragma region Global Const Value
constexpr float HalfRatio = 0.5f;
constexpr uint8 FullGauge = 100;
constexpr uint8 EmptyGauge = 0;
#pragma endregion

// Sets default values
ACharacterBase::ACharacterBase()
	:m_bVisiable(false), m_bTakeDamage(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Combat Stat    // 나중에 데이터를 받아서 초기화 할 값이라서 0.f로 초기화
	m_AttackRange = 0.f;
	m_AttackRadius = 0.f;
	m_RangedRadius = 0.f;
	m_DetectionRadius = 0.f;
	m_VisibleRadius = 0.f;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 800.f; // 600.f;

	// Speed Set ( MaxWalkSpeed : 600.f )
	m_OriginMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;


	// Stat
	//m_pStatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("StatComp"));
	m_pInventoryComp = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("Inventory"));
	m_pEquipmentComp = CreateDefaultSubobject<UMyEquipmentComponent>(TEXT("Equip"));
	m_pSkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("Skill"));
	m_pAbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability"));
	m_pAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	m_pAudioComp->bAutoActivate = false;  // 필요할 때만 Play


	m_EquipmentMeshComp.Init(nullptr, m_ItemFirstIndex);

	// 장비 생성
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		// Equipment Component
		m_EquipmentMeshComp[Index] = CreateDefaultSubobject<USkeletalMeshComponent>(m_PartTypeList[Index]);
		m_EquipmentMeshComp[Index]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, m_PartTypeList[Index]);
	}

	// (임시) 캐릭터 State랑 매핑해서 써보자 .
	for (uint8 State =0; State < static_cast<uint8>(EBehaviorState::End);State++)
	{
		StateTagMapping.Add(static_cast<EBehaviorState>(State), FGameplayTag::EmptyTag);
	}
	

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Init Property ( Spawn World ) 
	m_bVisiable = true;

	// OriginMaterial Init Setting
	m_OriginMaterial = GetMesh()->GetMaterial(0);


	// (임시) 값을 임의로 정해두는 구간이다. [ 기능이 생기면 수정하자 ] 
	m_CombatType = ECombatType::CT_Melee;

	// 여기에서 초기 데이터를 세팅해주자 
	SetupCharacterData();

	// 어빌리티 부여
	for (auto& AbilityData : m_AbilityData)
	{
		m_pAbilityComp->GrantAbility(AbilityData.Value.Get());
		
		// 시작 모드 태그(예: 전투 또는 평시) 보유 처리

	}


}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitEquipment();

	//Bind Section
	m_pSkillComp->m_EndSkill.AddUObject(this, &ACharacterBase::SetState);

	// 애님 인스턴스 불러오기 
	m_pAnimInstance = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (m_pAnimInstance != nullptr)
	{
		m_pAnimInstance->m_OnAttackHit.AddUObject(this, &ACharacterBase::AttackCheck);
		m_pAnimInstance->m_OnAttackEnd.AddUObject(this, &ACharacterBase::AttackEnd);
		m_pAnimInstance->m_OnFootStep.AddUObject(this, &ACharacterBase::PlayFootStep);

	}
	else
	{
		// (추측) CastChecked를 했었지만... 에디터에서 생성할때는 아마도 프로토 타입을 만들때는 GetMesh가 없어서 그때는 터지는 거같다..
	}

	//  Shiny & SoundCue 로드
	m_Shiny = LoadObject<UMaterial>(nullptr,TEXT("/Game/TopDown/Material/M_Shiny.M_Shiny"));
	if (m_Shiny != nullptr)
	{

	}

	m_FootstepCue = LoadObject<USoundCue>(nullptr,TEXT("/Game/FootstepSound/Sounds/Sound_Cues/Footsteps.Footsteps"));
	if (m_FootstepCue != nullptr && m_pAudioComp != nullptr)
	{
		m_pAudioComp->SetSound(m_FootstepCue);
	}

}

void ACharacterBase::SetupCharacterData()
{
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::Effect_Flick(FLinearColor Color, float Rate)
{
	if (true == GetWorld()->GetTimerManager().IsTimerActive(m_hEffectFlickTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer(m_hEffectFlickTimer, this, &ACharacterBase::EffectEnd_Flick, Rate, false);

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(m_Shiny, this);
	DynamicMaterial->SetVectorParameterValue(FName("Param_1"), Color);

	GetMesh()->SetMaterial(0, DynamicMaterial);

	//DEBUG

	//bool OK = false;
	//if (DynamicMaterial->IsVectorParameterUsedAsChannelMask(TEXT("Param_1"), OK) == true)
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : OK ")); }
	//else 
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : Failed ")); }
}

void ACharacterBase::EffectEnd_Flick()
{
	GetMesh()->SetMaterial(0, m_OriginMaterial);
}

#pragma region FootStep Function
void ACharacterBase::PlayFootStep()
{

	// 1) 시작점: 캐릭터 위치
	FVector Start = GetActorLocation();

	// 2) 방향: 업벡터의 반대
	FVector Direction = -GetActorUpVector();

	// 3) 거리 설정
	const float TraceDistance = 100.0f;
	FVector End = Start + Direction * TraceDistance;

	// 4) 트레이스 파라미터
	FHitResult HitResult;
	FCollisionQueryParams QueryParams(TEXT("FootStep"), true, this);
	//QueryParams.AddIgnoredActor(this);   // 자신은 무시
	QueryParams.bReturnPhysicalMaterial = true; // 피직스 머티리얼 반환

	// 5) 모든 채널 충돌 검사용 오브젝트 쿼리 파라미터
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	// 6) 라인 트레이스 실행
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		Start,
		End,
		ObjectQueryParams,
		QueryParams
	);

	if (bHit)
	{
		// 충돌 지점 얻기
		FVector HitLocation = HitResult.Location;
		//UE_LOG(LogTemp, Log, TEXT("Hit Location: %s"), *HitLocation.ToString());

		// 2) 물리 표면 타입 조회
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
		int32 SurfaceIndex = static_cast<int32>(SurfaceType);

		PlaySound_FootStep(SurfaceIndex);
		PlayEffect_FootStep(SurfaceIndex, HitLocation,this->GetActorRotation());


		// DEBUG
		DrawDebugLine(GetWorld(), Start, HitLocation, FColor::Green, false, 1.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), HitLocation, 5.0f, 12, FColor::Red, false, 1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No hit detected"));
	}


}

void ACharacterBase::PlaySound_FootStep(int32 SurfaceIndex)
{
	if (m_FootstepCue->IsValidLowLevelFast() == false)
	{
		UE_LOG(LogTemp, Log, TEXT(" FootStepCue is fail "));
	}

	m_pAudioComp->SetSound(m_FootstepCue);

	m_pAudioComp->SetBoolParameter(FName("step_play"), true);
	m_pAudioComp->SetBoolParameter(FName("rattle_play"), true);

	m_pAudioComp->SetFloatParameter(FName("step_volume"), 1.0f);
	m_pAudioComp->SetFloatParameter(FName("rattle_volume"), 0.0f);
	m_pAudioComp->SetIntParameter(FName("floor"), SurfaceIndex);
	m_pAudioComp->SetBoolParameter(FName("IsMale"), true);

	m_pAudioComp->Play();

	if (m_pAudioComp->IsPlaying() == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Play: FootStepSound"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Play: FootStepSound"));
	}
}

void ACharacterBase::PlayEffect_FootStep(int32 SurfaceIndex, FVector Location, FRotator ActorRotator)
{
	// Index 유효성 및 널 체크	// (임시) 객체 풀 없이 만들었다. 
	if (m_NiagaraEffects.IsValidIndex(SurfaceIndex) && m_NiagaraEffects[SurfaceIndex])
	{
		UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			m_NiagaraEffects[SurfaceIndex],
			Location,
			ActorRotator
		);
		Effect->SetRelativeScale3D((Effect->GetRelativeScale3D() * 3.f));
	}
	
}
#pragma endregion

const float ACharacterBase::GetMoveAnimationSpeed()
{
	if (m_OriginMoveSpeed > 0.f)
	{
		return GetCharacterMovement()->MaxWalkSpeed / m_OriginMoveSpeed;
	}
	else
	{
		return -1.f;
	}
}

void ACharacterBase::OnStopAttackMontage(float InBlendOutTime)
{
	if(m_pAnimInstance != nullptr)
		m_pAnimInstance->StopMontage(static_cast<uint8>(EMontageList::AttackMontage),InBlendOutTime);
}

void ACharacterBase::OnStopBehaviorMontage(float InBlendOutTime)
{
	if (m_pAnimInstance != nullptr)
		m_pAnimInstance->StopMontage(static_cast<uint8>(EMontageList::BehaviorMontage),InBlendOutTime);
}

void ACharacterBase::OnHitMontageEnded()
{
	if (m_CharacterState == EBehaviorState::Hit)
	{
		SetState(m_NextCharacterState);
	}
}

#pragma region Movement Function
void ACharacterBase::MoveToForward(float Value)
{
	// 값의 범위를 애니메이션 블랜더의 범위로 보간 해주자
	AddMovementInput(GetActorForwardVector(), Value);

	// 애니메이션 을 위한 보간 이라서 이동 후에 한다.
	m_Vertical = FMath::Clamp(Value, -1.f, 1.f);
}

void ACharacterBase::MoveToRight(float Value)
{
	// 값의 범위를 애니메이션 블랜더의 범위로 보간 해주자
	AddMovementInput(GetActorRightVector(), Value);

	// 애니메이션 을 위한 보간 이라서 이동 후에 한다.
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);
}

void ACharacterBase::SetMoveSpeed(float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// 캐릭터 이동속도
}

void ACharacterBase::ResetMoveSpeed()
{
	// 오버로드를 하지않고 명시적으로 Reset이라는 단어로 리셋임을 명시하였다.
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;
}

void ACharacterBase::SetMoveSpeed_Ratio(float Ratio)
{	
	// 값을 0.f~100.f으로 고정하기 위해서 Clamp해준다.
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed * FMath::Clamp(Ratio, 0.f, 100.f);
}

/// <summary>
///  비율 기반 Sprint
/// </summary>
/// <param name="WalkSpeedRatio"></param>
/// <param name="CameraSpeedRatio"></param>
/// <param name="HasState">캐릭터 상태가 포함되었는가</param>
void ACharacterBase::Sprint(float WalkSpeedRatio, float CameraSpeedRatio, bool HasState)
{
	// 상태가 포함된 기능인지 구분
	if (HasState == true)
	{
		if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
		{
			SetState(EBehaviorState::Running);
			m_pAnimInstance->SetIsSprint(true);
		}
		else
		{
			return;
		}
	}
	
	GetCharacterMovement()->MaxWalkSpeed = (m_OriginMoveSpeed * WalkSpeedRatio);		// 캐릭터 이동속도 
	// (임시) 게이지 카운터
	GetWorld()->GetTimerManager().SetTimer(m_hGaugeTimer,
		[this]() { this->m_pStatComp->AddSP(-1); }
		,0.1f, true);
}
/// <summary>
/// 값 기반 Sprint
/// </summary>
/// <param name="WalkSpeed"></param>
/// <param name="CameraSpeed"></param>
/// <param name="HasState">캐릭터 상태가 포함되었는가</param>
void ACharacterBase::SetSprint(float WalkSpeed, float CameraSpeed, bool HasState)
{
	// 상태가 포함된 기능인지 구분
	if (HasState == true)
	{
		if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
		{
			SetState(EBehaviorState::Running);
			m_pAnimInstance->SetIsSprint(true);
		}
		else
		{
			return;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// 캐릭터 이동속도
	
	
}
/// <summary>
/// 
/// </summary>
/// <param name="HasState">캐릭터 상태가 포함되었는가</param>
void ACharacterBase::StopSprint(bool HasState)
{
	// 상태가 포함된 기능인지 구분
	if (HasState == true)
	{
		if (m_Mode == ECharacterMode::Idle)
		{
			SetState(EBehaviorState::Idle);
			m_pAnimInstance->SetIsSprint(false);
		}
		else if(m_Mode == ECharacterMode::Battle)
		{
			SetState(EBehaviorState::Battle);
			m_pAnimInstance->SetIsSprint(false);
		}
		else
		{
			// 잘못된 상태 정지이다.
			UE_LOG(LogTemp, Log, TEXT("Invalid input state detected."));
			return;
		}
	}
	
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;		// 캐릭터 이동속도
	
	// (임시) 게이지 카운터
	GetWorld()->GetTimerManager().ClearTimer(m_hGaugeTimer);
}
#pragma endregion

#pragma region Inventory Function
const int32 ACharacterBase::GetItemType(int32 Index)
{
	return m_pInventoryComp->GetItemType(Index);
}
/// <summary>
/// 캐릭터 단에서 UseItem을 한다.
/// </summary>
/// <param name="Index">아이템 리스트에서의 번호 </param>
void ACharacterBase::UseItem(int32 Index)
{
	if (m_pInventoryComp == nullptr)
		return;

	FBaseStatusData Status{};
	switch (m_pInventoryComp->UseItem(Status, Index))
	{
		case static_cast<uint8>(EItemType::ConsumableItem) :
		{
			// Add Status [ 영구 증가 ] 
			m_pStatComp->AddModifierStat(Status);
			break;
		}
	}


	/* 이제 안쓰임. Legacy*/
	/*
	// 일단 값들이 int32라서 배열로 쭉 받긴 하는데 이게 맞는지... 
	TArray<int32> ItemEffect = m_pInventoryComp->UseItem(Index);
	if (ItemEffect.IsEmpty() == false)
	{
		switch (ItemEffect[0])
		{
		case static_cast<int32>(EItemEffectType::PlusHP):
		{
			m_pStatComp->AddHP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusMP):
		{
			m_pStatComp->AddMP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusSP):
		{
			m_pStatComp->AddSP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusAttack):
		{
			FBaseStatusData Stat{};
			Stat.Attack = ItemEffect[2];
			m_pStatComp->AddBaseStat(Stat);

			break;
		}
		case static_cast<int32>(EItemEffectType::PlusDefence):
		{
			FBaseStatusData Stat{};
			Stat.Defence = ItemEffect[2];
			m_pStatComp->AddBaseStat(Stat);

			break;
		}

		default:
			break;
		}

	}
	*/
}
/// <summary>
/// 캐릭터 단에서 DropItem을 한다.
/// </summary>
/// <param name="Index"></param>
/// <param name="Pos"></param>
void ACharacterBase::DropItem(int32 Index, FVector Pos)
{
	if (m_pInventoryComp == nullptr)
		return;
	// 장비 아이템이면 해제 해준다.
	if(Index < m_ItemFirstIndex)	
	{
		UnEquip(Index);
	}
	// 여기서 아이템 액터 생성해주기 .
	UMyGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (GameSubsystem != nullptr)
	{
		// 캐릭터의 위치에 아이템 드랍.
		GameSubsystem->GetItemActor(m_pInventoryComp->DropItem(Index), GetTransform());
	}

}

bool ACharacterBase::Equip(uint8 Part)
{
	// 장비가 아닌것들은 false반환 [ 스켈레탈 메시가 없으면 ] 
	FBaseStatusData Status{};	
	Status = m_pInventoryComp->GetItemStat(Part);


	if (m_pInventoryComp->GetSkeletalMesh_Equipment(Part).IsValid() == false)
	{
		m_pInventoryComp->GetSkeletalMesh_Equipment(Part).LoadSynchronous();
	}
	FString Path = m_pInventoryComp->GetSkeletalMesh_Equipment(Part)->GetPathName();
	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *(Path), NULL, LOAD_None, NULL);


	if (SkeletalMesh != nullptr)
	{
		// Debug
		UE_LOG(LogTemp, Log, TEXT("Equip Attack : %d "), m_pStatComp->GetTotalStat().Attack);
		UE_LOG(LogTemp, Log, TEXT("Equip Defence : %d "), m_pStatComp->GetTotalStat().Defence);

		// Add Status 
		m_pStatComp->AddModifierStat(Status);
		// Set SkeletalMesh
		m_EquipmentMeshComp[Part]->SetSkeletalMesh(SkeletalMesh);
		return true;
	}
	else
	{
		return false;
	}
}

bool ACharacterBase::UnEquip(uint8 Part)
{
	// 나중에 장비 해체 불가 상태를 만들어서 처리하기 위한 반환값.
	// 문제는 같은 해체할 때도 같은 인덱스의 장비가 맞는지 확인을 먼저 해야할 듯하다. 
	FBaseStatusData Status{};
	if (m_pInventoryComp->UseEquipment(Status, Part) == true)
	{
		// Debug
		UE_LOG(LogTemp, Log, TEXT("UnEquip Attack : %d "), m_pStatComp->GetTotalStat().Attack);
		UE_LOG(LogTemp, Log, TEXT("UnEquip Defence : %d "), m_pStatComp->GetTotalStat().Defence);

		// Add Status 
		m_pStatComp->AddModifierStat(-Status);
		// Set SkeletalMesh
		m_EquipmentMeshComp[Part]->SetSkeletalMesh(nullptr);
		return true;

	}

	return false;
}

void ACharacterBase::InitEquipment()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (CharacterMesh == nullptr)
	{
		return;
	}

	// 장비 초기화 
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		// Equipment Component

		if (CharacterMesh->DoesSocketExist(m_PartTypeList[Index]) == true)
		{
			// 왜인지 모르겠지만 SetupAttachment는 작동이 안된다.
			//m_EquipmentMeshComp[Index]->SetupAttachment(CharacterMesh, m_PartTypeList[Index]);
			if (m_EquipmentMeshComp[Index]->AttachToComponent
			(
				CharacterMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				m_PartTypeList[Index]
			) == false)
			{
				UE_LOG(LogTemp, Log, TEXT(" Not Attached "));
			}

			// 제대로 부착되는지 로그 남기기.
			if (GetMesh() == CastChecked<USkeletalMeshComponent>(m_EquipmentMeshComp[Index]->GetAttachParent()))
			{
				UE_LOG(LogTemp, Log, TEXT("Equipment successfully attached to character!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Attachment failed!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Not Found Socket Name!!!"));
		}



		// 현재는 Load하고 있는 장비창이 없지만, Load후에 초기 장착 세팅해주는 부분.
		/*

		*/

	}

}
#pragma endregion

#pragma region Damage Function
float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_bTakeDamage == false || m_pStatComp->GetHP() == 0)
	{
		return 0.f;
	}
		
	OnHit();

	m_pStatComp->OnAttacked(DamageAmount);
	return DamageAmount;
}

bool ACharacterBase::CheckLineTrace_Single(FVector Start, FVector End, ECollisionChannel Channel, float TransientDamage)
{
	FHitResult HitResult;

	FCollisionQueryParams CQP("TraceParams", true, this);
	CQP.bReturnPhysicalMaterial = false;

	bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, Channel, CQP);

	/* Debug */
	//FColor DrawColor;
	//if (bResult)
	//	DrawColor = FColor::Green;
	//else
	//	DrawColor = FColor::Red;
	//DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 2.0f);

	if (bResult && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

		FDamageEvent DamageEvent;
		float Damage = m_pStatComp->GetAttack() + TransientDamage;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);

	}

	return bResult;
}

bool ACharacterBase::CheckLineTrace_Multi(FVector Start, FVector End, ECollisionChannel Channel, float TransientDamage)
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CQP("TraceParams", true, this);
	CQP.bReturnPhysicalMaterial = false;

	bool bResult = GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, Channel, CQP);

	/* Debug */
	//FColor DrawColor;
	//if (bResult)
	//	DrawColor = FColor::Green;
	//else
	//	DrawColor = FColor::Red;
	//DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 2.0f);

	// 선별 작업.
	TArray<AActor*> UniqueResult;
	for (auto& Src : HitResults)
	{
		UniqueResult.AddUnique(Src.GetActor());
	}

	if (bResult == true)
	{
		for (auto HitActor : UniqueResult)
		{
			if (HitActor != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitActor->GetName());

				FDamageEvent DamageEvent;
				float Damage = m_pStatComp->GetAttack() + TransientDamage;
				HitActor->TakeDamage(Damage, DamageEvent, GetController(), this);
			}
		}
	}

	return bResult;
}

bool ACharacterBase::CheckSweep_Single(FCollisionShape CollisionShape, ECollisionChannel Channel, float TransientDamage)
{
	FHitResult HitResult;
	FCollisionQueryParams CQP("SweepParams", true, this);


	FVector Vec = GetActorForwardVector() * m_AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = m_AttackRange * 0.5f + m_AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();


	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * m_AttackRange,
		Rotation,
		Channel,
		CollisionShape,
		CQP);

	/* Debug */
	//FColor DrawColor;
	//if (bResult)
	//	DrawColor = FColor::Green;
	//else
	//	DrawColor = FColor::Red;
	//
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, m_AttackRadius,
	//	Rotation, DrawColor, false, 2.f);


	if (bResult && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

		FDamageEvent DamageEvent;
		float Damage = m_pStatComp->GetAttack() + TransientDamage;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);

	}


	return bResult;
}

bool ACharacterBase::CheckSweep_Multi(FCollisionShape CollisionShape, ECollisionChannel Channel, float TransientDamage)
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CQP("SweepParams", false, this);


	FVector Vec = GetActorForwardVector() * m_AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = m_AttackRange * 0.5f + m_AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();


	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * m_AttackRange,
		Rotation,
		Channel,
		CollisionShape,
		CQP);

	/* Debug */
	//FColor DrawColor;
	//if (bResult)
	//	DrawColor = FColor::Green;
	//else
	//	DrawColor = FColor::Red;
	//
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, m_AttackRadius,
	//	Rotation, DrawColor, false, 2.f);

	TArray<AActor*> UniqueResult;
	for (FHitResult& Src : HitResults)
	{
		UniqueResult.AddUnique(Src.GetActor());
	}

	if (bResult == true)
	{
		for (AActor* HitResult : UniqueResult)
		{
			if (HitResult != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult->GetName());

				FDamageEvent DamageEvent;
				float Damage = m_pStatComp->GetAttack() + TransientDamage;
				HitResult->TakeDamage(Damage, DamageEvent, GetController(), this);
			}
		}


	}


	return bResult;
}

bool ACharacterBase::CheckOverlap(FCollisionShape CollisionShape, ECollisionChannel Channel, float TransientDamage)
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CQP("OverlapParams", true, this);

	FVector Vec = GetActorForwardVector() * m_AttackRange;
	FVector Center = GetActorLocation() + Vec * 1.0f;
	float HalfHeight = m_AttackRange * 0.5f + m_AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorRightVector()).ToQuat();



	bool bResult = GetWorld()->OverlapMultiByChannel(
		OUT OverlapResults,
		GetActorLocation() + Vec,
		Rotation,
		Channel,
		CollisionShape,
		CQP);

	/* Debug */
	//FColor DrawColor;
	//if (bResult)
	//	DrawColor = FColor::Green;
	//else
	//	DrawColor = FColor::Red;
	//
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, m_AttackRadius,
	//	Rotation, DrawColor, false, 2.f);


	// 중복 제거를 위해서 한번 걸러준다.
	TArray<AActor*> UniqueResult;
	for (FOverlapResult Src : OverlapResults)
	{
		UniqueResult.AddUnique(Src.GetActor());
	}

	if (bResult == true)
	{
		for (AActor* HitResult : UniqueResult)
		{

			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult->GetName());

			FDamageEvent DamageEvent;
			float Damage = m_pStatComp->GetAttack() + TransientDamage;
			HitResult->TakeDamage(Damage, DamageEvent, GetController(), this);
		}
	}

	return bResult;
}

#pragma endregion

void ACharacterBase::InitalizeMeshComponent()
{
	// 처음 세팅용이지만 없앨수도 있다.
	SetMainMesh();
}

void ACharacterBase::SetMainMesh()
{
	UMyGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (GameSubsystem != nullptr)
	{
		USkeletalMesh* SkeletalMesh = GameSubsystem->GetCharacterInfo_MeshData(m_CharacterName);
		if (SkeletalMesh != nullptr)
		{
			GetMesh()->SetSkeletalMesh(SkeletalMesh);
		}
	}
}

void ACharacterBase::ApplyHidden(const bool bHide)
{
	AActor::SetHidden(bHide);

	m_bVisiable = !bHide;
	SetActorHiddenInGame(!m_bVisiable);
	SetActorEnableCollision(m_bVisiable);
	SetActorTickEnabled(m_bVisiable);
}

#pragma region Behavior Interface
void ACharacterBase::SetState(const EBehaviorState State)
{
	// 상태를 전환할때 Set함수를 쓰는 이유는 노티와 디버그를 편하게 하기 위해서 

#pragma region Debug Section
	//if (State == EBehaviorState::Idle && m_CharacterState != EBehaviorState::Idle)
	//{
	//	UE_LOG(LogTemp, Log, TEXT(" WoW Yo Gi It Ne "));
	//}
#pragma endregion


	m_CharacterState = State;
	OnChangeBehavior.Broadcast();

}

void ACharacterBase::SetMode(const ECharacterMode Mode)
{
	if (m_Mode != Mode)
	{
		m_Mode = Mode;
		OnChangeMode.Broadcast(static_cast<uint8>(m_Mode));
	}
}

void ACharacterBase::SetState(const uint8 State)
{
	// 상태를 전환할때 Set함수를 쓰는 이유는 노티와 디버그를 편하게 하기 위해서 

#pragma region Debug Section
	//if (static_cast<EBehaviorState>(State) == EBehaviorState::Idle && m_CharacterState != EBehaviorState::Idle)
	//{
	//	UE_LOG(LogTemp, Log, TEXT(" WoW Yo Gi It Ne "));
	//}
#pragma endregion

	m_CharacterState = static_cast<EBehaviorState>(State);
	OnChangeBehavior.Broadcast();
}

void ACharacterBase::OnIdle()
{
}

void ACharacterBase::OnBattle()
{
}

void ACharacterBase::Attack()
{
	if (m_CharacterState == EBehaviorState::Idle ||
		m_CharacterState == EBehaviorState::Battle)
	{
		FName Tag(*FString::Printf(TEXT("%s.%s"), TEXT("Ability.Attack"), *m_pAnimInstance->GetAttackMontageName(m_AttackIndex).ToString()));
		
		FAbilityHandle Handle;
		FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(Tag);
		m_pAbilityComp->TryActivateByTag(GameplayTag, Handle);
		//m_pAnimInstance->PlayAttackMontage(m_AnimationSpeed);
		//m_pAnimInstance->JumpToSection_Attack(m_AttackIndex);

		// (임시) 장비 애니메이션도 일체화 하기 
		for (auto elem : m_EquipmentMeshComp)
		{
			if (elem->GetAnimInstance() == nullptr) continue;
			Cast<UAnimInstanceBase>(elem->GetAnimInstance())->PlayMontage(static_cast<uint8>(EMontageList::AttackMontage), m_AnimationSpeed);
			Cast<UAnimInstanceBase>(elem->GetAnimInstance())->JumpToSection(m_pAnimInstance->GetAttackMontageName(m_AttackIndex), static_cast<uint8>(EMontageList::AttackMontage));
		}

		m_AttackIndex = (m_AttackIndex + 1) % m_pAnimInstance->GetMontageSectionNum(static_cast<uint8>(EMontageList::AttackMontage));

		// Setting State
		SetState(EBehaviorState::Attacking);
		SetNextState(EBehaviorState::Battle);

		if (m_AggroGauge == FullGauge)
		{
			// Effect
			Effect_Flick(FLinearColor::White);
		}
	}
}

void ACharacterBase::AttackCheck()
{
	if (m_CharacterState == EBehaviorState::Attacking)
	{
		// 어그로 게이지가 풀이면 데미지가 한번 더 가산되도록...
		float Damage = (m_AggroGauge == FullGauge) ? m_pStatComp->GetAttack() : 0.0f;
		// 어그로 게이지 전부 소모
		if (Damage > 0.0f)
		{
			m_AggroGauge = EmptyGauge;
		}
		float HalfHeight = (m_AttackRange * HalfRatio) + m_AttackRadius;
		if (CheckSweep_Multi(FCollisionShape::MakeCapsule(m_AttackRadius, HalfHeight), m_Enemy, Damage) == false)
		{
			// fail
			return;
		}
	}
}

void ACharacterBase::AttackEnd()
{
	// 매번 Attack Montage의 Section이 끝나면 Notify에 의해서 호출 
	if (m_CharacterState == EBehaviorState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_pAnimInstance->StopMontage(static_cast<uint8>(EMontageList::AttackMontage));
		
		//(임시) 장비 애니메이션 동기화
		for (auto elem : m_EquipmentMeshComp)
		{
			if (elem->GetAnimInstance() == nullptr) continue;
			Cast<UAnimInstanceBase>(elem->GetAnimInstance())->StopMontage(static_cast<uint8>(EMontageList::AttackMontage));
		}

		SetState(m_NextCharacterState);
	}
}

void ACharacterBase::Death()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	SetState(EBehaviorState::Die);
	SetNextState(EBehaviorState::Die);
	//m_pAnimInstance->PlayBehaviorMontage(m_AnimationSpeed);
	//m_pAnimInstance->JumpToSection_Behavior(TEXT("Death"));
	m_pAnimInstance->SetIsLive(false);
}

void ACharacterBase::DeathEnd()
{
}

void ACharacterBase::OnHit()
{
	if (m_Mode == ECharacterMode::Idle)
	{
		SetMode(ECharacterMode::Battle);
	}

	SetState(EBehaviorState::Hit);
	SetNextState(EBehaviorState::Battle);
	//m_pAnimInstance->PlayHitMontage(m_AnimationSpeed);
	//m_pAnimInstance->JumpToSection_Hit(TEXT("Hit"));

	// Effect
	Effect_Flick(FLinearColor::Red);

}

void ACharacterBase::OnDefense()
{
}

void ACharacterBase::StopDefense()
{
}

void ACharacterBase::Defense_Hit()
{
}

void ACharacterBase::OnDetect()
{
}

void ACharacterBase::DetectEnd()
{
}

void ACharacterBase::OnAlert()
{
}

void ACharacterBase::OnCommand(uint8 Command)
{
}

void ACharacterBase::BehaviorInterrupted(uint8 State)
{
	// 만약에 중간에 행동이 종료될때 처리를 위해서 만들어 두었다.

}

void ACharacterBase::LookDirection(float Value)
{
	// 값의 범위를 애니메이션 블랜더의 범위로 보간 해주자
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);

	//Debug
	UE_LOG(LogTemp, Log, TEXT("Horizontal : %f "), m_Horizontal);
}

void ACharacterBase::SetUpdateBehaviorDelegate(const FOnUpdateBehavior& Delegate)
{
}

bool ACharacterBase::IsOnTriggerEscape()
{
	return false;
}
#pragma endregion