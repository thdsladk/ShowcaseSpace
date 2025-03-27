// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCharacter.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_Goblin.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyAIController.h"
#include "MonsterStatComponent.h"
#include "PawnWidget.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

#include "MyEquipment.h"
#include "DamageFontWidget.h"
#include "EmotionWidget.h"

#include "Engine/AssetManager.h"
#include "NavigationSystem.h"

// Sets default values
ANPCharacter::ANPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// ĸ�� ������� �ʿ��ϸ� �ٽ� ����������.
	//// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Skeletal Mesh
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
	//
	//if (SM.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SM.Object);
	//}

	// Stat
	m_pStatComp = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("Stat"));


	// HP bar
	m_HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	m_HpBar->SetupAttachment(GetMesh());
	m_HpBar->SetRelativeLocation(FVector{ 0.f,0.f,200.f });
	m_HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Game/TopDown/UI/WBP_HPBar.WBP_HPBar_C"));
	if (UW.Succeeded())
	{
		m_HpBar->SetWidgetClass(UW.Class);
		m_HpBar->SetDrawSize(FVector2D{ 200.f,50.f });
	}

	// Emotion
	m_Emotion = CreateDefaultSubobject<UWidgetComponent>(TEXT("EMOTION"));
	m_Emotion->SetupAttachment(GetMesh());
	m_Emotion->SetRelativeLocation(FVector{ 0.f,0.f,250.f });
	m_Emotion->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> EW(TEXT("/Game/TopDown/UI/WBP_Emotion.WBP_Emotion_C"));
	if (EW.Succeeded())
	{
		m_Emotion->SetWidgetClass(EW.Class);
		m_Emotion->SetDrawSize(FVector2D{ 25.f,25.f });
	}
	
	static ConstructorHelpers::FClassFinder<UDamageFontWidget> DFW(TEXT("/Game/TopDown/Blueprints/BP_DamageFont.BP_DamageFont_C"));
	if (DFW.Succeeded())
	{
		m_DamageFont = DFW.Class;
	}



	// AI Set
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
// Called when the game starts or when spawned
void ANPCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	m_NPCType = FName("Goblin_Red-1");
	FMonsterData* MonsterData = Cast<UMyGameInstance>(GetGameInstance())->GetMonsterStatData(m_NPCType.ToString());
	//// DuplicateObject�� ���ؼ� ������ �޽ð�ü�� ��� �޸� ������ �ұ� ���� ��������� �ϴ°��ϱ�? 
	//// UObject�� �ڽ��̶� �ϴ� ������ �÷��Ͱ� �����Ұ����� ���δ�. 
	//GetMesh()->SetSkeletalMesh(DuplicateObject<USkeletalMesh>(MonsterData->SkeletalMesh, this));

	//AttachmentEquipment Weapon1
	FName WeaponSocket(TEXT("Weapon_Socket"));
	m_EquipmentLeft = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());

	if (nullptr != m_EquipmentLeft)
	{
		m_EquipmentLeft->SetItem(MonsterData->LItemID);
		m_EquipmentLeft->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
		m_EquipmentLeft->OnEquip();

		m_EquipmentLeft->FinishSpawning(FTransform());
	}

	//AttachmentEquipment Weapon2
	FName ShieldSocket(TEXT("Shield_Socket"));
	m_EquipmentRight = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());

	if (nullptr != m_EquipmentRight)
	{
		m_EquipmentRight->SetItem(MonsterData->RItemID);
		m_EquipmentRight->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ShieldSocket);
		m_EquipmentRight->OnEquip();

		m_EquipmentRight->FinishSpawning(FTransform());
	}



	//DEBUG 
	GetWorld()->GetTimerManager().SetTimer(m_hDebug, this, &ANPCharacter::Debug, 0.1f, true);

}
// Called every frame
void ANPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Debug

	//UE_LOG(LogTemp, Log, TEXT("MonState : %d"),m_CharacterState);
}
// Called to bind functionality to input
void ANPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������ NPC�� ��ǲ ���� ������Ʈ�� ���� ������ ����. 

}

void ANPCharacter::PostInitializeComponents()
{
	// ������Ʈ ���� �ʱ�ȭ�� ���� 
	Super::PostInitializeComponents();


	m_AnimInstance = CastChecked<UAnimInstance_Goblin>(GetMesh()->GetAnimInstance());
	// �ִϸ��̼ǿ��� ��Ÿ�ְ� ������ ������ ȣ���ϴ� ��������Ʈ�� ���ξ���.
	//m_AnimInstance->OnMontageEnded.AddDynamic(this, &ANPCharacter::OnAttackMontageEnded);
	// �ִϸ��̼� Ư�� �������� ȣ���ϵ��� �ؼ� ���ϴ� ���� �Ǵ� �������� ȣ�� �ϵ��� ����
	m_AnimInstance->m_OnAttackHit.AddUObject(this, &ANPCharacter::AttackCheck);
	m_AnimInstance->m_OnAttackEnd.AddUObject(this, &ANPCharacter::AttackEnd);

	m_AnimInstance->m_OnDeathPoint.AddUObject(this, &ANPCharacter::OnDeathMontageEnded);

	// ���ݿ��� OnDeathCheck�� Broadcast �ȴٸ� Death�Լ� ȣ�� 
	m_pStatComp->OnDeathCheck.AddUObject(this, &ANPCharacter::Death);

	// Combat Stat				// �׽��ÿ� ���̴�. ���� �޾Ƽ� ��ü���� �ٸ��� �����ؾ��Ѵ�.
	m_AttackRange = 50.f;
	m_AttackRadius = 50.f;
	m_DefenseRadius = 150.f;
	m_DetectionRadius = 800.f;
	m_VisibleRadius = 1200.f;


	m_HpBar->InitWidget();
	m_Emotion->InitWidget();

	// TODO
	auto HpWidget = Cast<UPawnWidget>(m_HpBar->GetUserWidgetObject());
	if (HpWidget)
		HpWidget->BindHp(m_pStatComp);


	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &ANPCharacter::NPCMeshLoadCompleted));


}

void ANPCharacter::OnIdle()
{
	m_CharacterState = EBehaviorState::Idle;
	m_NextCharacterState = EBehaviorState::Idle;
	m_Mode = ECharacterMode::Idle;

	ResetMoveSpeed();
	LookDirection(0.f);


}

void ANPCharacter::OnBattle()
{
	m_CharacterState = EBehaviorState::Battle;
	m_NextCharacterState = EBehaviorState::Battle;
	m_Mode = ECharacterMode::Battle;

	SetMoveSpeed((m_OriginMoveSpeed *0.5f));
}

void ANPCharacter::Attack()
{
	m_AnimInstance->PlayAttackMontage();
	m_AnimInstance->JumpToSection_Attack(m_AttackIndex);
	m_AttackIndex = (m_AttackIndex + 1) % 3; // Attack ����� 2�� �̹Ƿ�

	m_CharacterState = EBehaviorState::Attacking;
	m_NextCharacterState = EBehaviorState::Battle;
}

void ANPCharacter::AttackCheck()
{
	if (m_CharacterState != EBehaviorState::Attacking)
		return;

	FHitResult HitResult; 
	FCollisionQueryParams Params(NAME_None, false, this);


	FVector Vec = GetActorForwardVector() * m_AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = m_AttackRange * 0.5f + m_AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();


	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * m_AttackRange,
		Rotation,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(m_AttackRadius,HalfHeight),
		Params);


	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, m_AttackRadius,
		Rotation, DrawColor, false, 2.f);


	if (bResult && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(m_pStatComp->GetAttack(), DamageEvent, GetController(), this);

	}
}

void ANPCharacter::AttackEnd()
{
	if (m_CharacterState == EBehaviorState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_AnimInstance->StopAttackMontage();
		m_CharacterState = m_NextCharacterState;
	}
}

void ANPCharacter::Death()
{
	if (m_CharacterState < EBehaviorState::Die)
	{
		m_CharacterState = EBehaviorState::Die;
		m_NextCharacterState = EBehaviorState::Die;
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(TEXT("Death_B"));

		m_AnimInstance->SetIsLive(false);
	}
}

void ANPCharacter::DeathEnd()
{
	if(m_CharacterState == EBehaviorState::Die)
	{

#pragma region DEBUG
		//if (GEngine) 
		//{
		//	GEngine->AddOnScreenDebugMessage
		//	(-1, 1.0f, FColor::Red, FString::FromInt(static_cast<int32>(m_fCurrentOpacity * 100.f)));
		//}
#pragma endregion

		if (m_fCurrentOpacity <= 0.f)
		{
			m_CharacterState = EBehaviorState::End;
			m_NextCharacterState = EBehaviorState::End;
			OnDeathEnd.Broadcast();
			GetWorld()->GetTimerManager().ClearTimer(m_hDeathTimer);

			// �޽��� ������ �ִ� ������ �ڽĵ��� ��� ���� ó��.
			TArray<USceneComponent*> AttachedActors = GetMesh()->GetAttachChildren();
			for (auto elem : AttachedActors)
			{
				elem->SetVisibleFlag(false);
				elem->SetHiddenInGame(true);
			}

			//this->SetActorEnableCollision(false);
			GetMesh()->SetHiddenInGame(true);
			m_HpBar->SetVisibility(false);
			return;
		}
		else
		{

			//DEBUG
			//UE_LOG(LogTemp, Log, TEXT("%s"), *(m_AnimInstance->Montage_GetCurrentSection().ToString()));


			// �̰� �׺���̼� �޽��� �������� ����Ʒ��� �̵��� �ȵǴ°� ����.
			//GetCharacterMovement()->AddForce(FVector{ 0.f,0.f,-10.f });
			//GetCharacterMovement()->MoveSmooth(FVector{ 0.f,0.f,-10.f }, 1.f);

			// �������� ��Ƽ���� �ν��Ͻ��� �����մϴ�.
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(m_OriginMaterial, this);
			// ���� ���� ���� ������ 0.1f��ŭ ���ҽ�ŵ�ϴ�.
			if (nullptr != DynamicMaterial)
			{
				m_fCurrentOpacity -= 0.05f;
				DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), (FLinearColor::White * m_fCurrentOpacity));
				// �޽ÿ� �������� ������ ��Ƽ���� �ν��Ͻ��� �����մϴ�.
				GetMesh()->SetMaterial(0, DynamicMaterial);
			}
			else
			{
				//DEBUG
				UE_LOG(LogTemp, Log, TEXT("DynamicMaterial : is nullptr"));
			}
		}


	} 
}

void ANPCharacter::OnHit()
{	
	m_CharacterState = EBehaviorState::Hit;
	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FName(FString::Printf(TEXT("Hit"))));

	// Effect
	Effect_Flick(FLinearColor::Red);
}

void ANPCharacter::OnDefense()
{
	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FName(FString::Printf(TEXT("Defense"))));

	m_CharacterState = EBehaviorState::Defense;
	m_NextCharacterState = EBehaviorState::Battle;
}

void ANPCharacter::StopDefense()
{
	m_AnimInstance->StopDeathMontage();
	


	m_CharacterState = m_NextCharacterState;
}

void ANPCharacter::Defense_Hit()
{
	m_CharacterState = EBehaviorState::Hit;

	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FName(FString::Printf(TEXT("Defense_Hit"))));

	// Effect
	Effect_Flick(FLinearColor::Black);
}

void ANPCharacter::OnDetect()
{
	if (m_CharacterState == EBehaviorState::Idle)
	{
		m_CharacterState = EBehaviorState::Detect;

		m_NextCharacterState = EBehaviorState::Alert;
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(TEXT("Detect"));
		
		// Effect
		Effect_Flick(FLinearColor::White);
	
	}

}

void ANPCharacter::OnAlert()
{

	if (FMath::IsNearlyEqual(m_AlertTime,-1.0) == true)
	{
		// ��� ������ ���ð��� �����ϱ� ���ؼ� Ÿ�̸� Ȱ��
		GetWorld()->GetTimerManager().SetTimer(m_hAlertTimer, this, &ANPCharacter::OnAlert, 1.0f, true);
		m_AlertTime = m_AlertCoolTime;
		
		// (�ӽ�) �̵��ӵ� ���� .
		SetMoveSpeed((m_OriginMoveSpeed / 3.f));
	}
	else if (FMath::IsNearlyZero(m_AlertTime) == true)
	{
		// ���� ����
		OnAlertEnd.Broadcast();
		OnBattle();

		m_AlertTime = -1.0;
		GetWorld()->GetTimerManager().ClearTimer(m_hAlertTimer);
	}
	else
	{
		if (FMath::RandRange(0, 9) < 2)
		{
			// 20% Ȯ���� �ð� 2�� ���� 
			m_AlertTime += -2.0;
		}
		else
		{
			// 80% Ȯ���� �ð� 1�� ����
			m_AlertTime += -1.0;
		}
	}


}

void ANPCharacter::OnCommand(uint8 Command)
{
	FCommandData* CommandData = CastChecked<UMyGameInstance>(GetGameInstance())->GetCommandData(Command);

	auto Emotion = Cast<UEmotionWidget>(m_Emotion->GetUserWidgetObject());
	if (Emotion)
		Emotion->StartEmotion(CommandData->Icon,3.f);
}

bool ANPCharacter::IsOnTriggerEscape()
{
	// (�ӽ�) 0.2f �� ����̴ϱ� ������ ���� ���� �Ű������� �ްų�  ��� ������ �����ؼ� ����������ҵ��ϴ�. 
	return (m_pStatComp->GetHPRatio() <= 0.2f);
}

void ANPCharacter::OnStopAttackMontage(float InBlendOutTime)
{
	m_AnimInstance->StopAttackMontage(InBlendOutTime);
}

void ANPCharacter::OnStopDeathMontage(float InBlendOutTime)
{
	m_AnimInstance->StopDeathMontage(InBlendOutTime);
}

void ANPCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(m_CharacterState == EBehaviorState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_AnimInstance->StopAttackMontage();
		m_CharacterState = m_NextCharacterState;
		
	}
}

void ANPCharacter::OnDeathMontageEnded()
{
 
	
	//if (GEngine) {
	//	
	//	if (m_AnimInstance->GetMontageInst(1) == nullptr)
	//	{
	//		GEngine->AddOnScreenDebugMessage
	//		(-1, 10.0f, FColor::Red, TEXT("Montage Pointer : Nullptr"));
	//	}
	//	else
	//	{
	//		FString str(TEXT("<< Animation >> : "));
	//		str += m_AnimInstance->Montage_GetCurrentSection(m_AnimInstance->GetMontageInst(1)).ToString();
	//		GEngine->AddOnScreenDebugMessage
	//		(-1, 10.0f, FColor::Green, str);
	//		GEngine->AddOnScreenDebugMessage
	//		(-1, 10.0f, FColor::White, strState[m_CharacterState]);
	//	}
	//
	//}

	switch (m_CharacterState)
	{
		case EBehaviorState::Hit:
		{
			m_CharacterState = m_NextCharacterState;

			break;
		}
		case EBehaviorState::Defense:
		{
			m_CharacterState = m_NextCharacterState;
			OnDefenseEnd.Broadcast();

			break;
		}

		case EBehaviorState::Detect:
		{
			m_CharacterState = m_NextCharacterState;
			
			OnDetectEnd.Broadcast();
			break;
		}
		case EBehaviorState::Die:
		{
			this->SetActorEnableCollision(false);
			
			
			GetWorld()->GetTimerManager().SetTimer(m_hDeathTimer, this, &ANPCharacter::DeathEnd, 1.f, true);
			break;
		}
		case EBehaviorState::End:
		{

			break;
		}

		default :
		{
			
			break;
		}
	}


}

float ANPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_CharacterState == EBehaviorState::Die)
		return 0.f;

	// NextState
	m_NextCharacterState = EBehaviorState::Battle;
	m_Mode = ECharacterMode::Battle;

	// Overcome �غ��ϴ�. 10%�� Ȯ���� ȸ�� Dodge
	int32 Random = FMath::RandRange(0, 9);
	if (Random < 1)
	{
		// �����ϸ� �ٷ� ���� ����.
		m_CharacterState = EBehaviorState::Battle;
		DamageAmount = 0.f;
	}
	else 
	{
		if (m_CharacterState == EBehaviorState::Defense)
		{
			// ��� ����. 
			DamageAmount *= 0.5f;
			
			// ��� ���°� ����Ǿ��ٴ°� �˸���.
			OnDefenseEnd.Broadcast();
			//m_CharacterState = EBehaviorState::Hit;	// Defense_Hit�Լ� �ȿ��ٰ� ���� ��ȯ�� �־���.
			Defense_Hit();
			//m_pStatComp->OnHPChanged.AddLambda([this, DamageAmount]()
			//	{
			//		if (DamageAmount > 0)
			//		{
			//			this->Defense_Hit();
			//		}
			//	});

		}
		else
		{
			// Change State
			//m_CharacterState = EBehaviorState::Hit;	// Hit�Լ� �ȿ��ٰ� ���� ��ȯ�� �־���.
			OnHit();
			//m_pStatComp->OnHPChanged.AddLambda([this, DamageAmount]()
			//	{
			//		if (DamageAmount > 0)
			//		{
			//			this->OnHit();
			//		}
			//	});
		}
		


	}

	// Set Stat
	m_pStatComp->OnAttacked(DamageAmount);

	// DamageFont 
	FVector OverHead = ANPCharacter::GetMesh()->GetSocketLocation(TEXT("OverHead"));
	FVector2D OverHead2D;
	if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), OverHead, OverHead2D))
	{
		UUserWidget* DFW = CreateWidget(GetWorld(), m_DamageFont, TEXT(""));
		DFW->SetPositionInViewport(OverHead2D, true);
		DFW->AddToViewport();
		Cast<UDamageFontWidget>(DFW)->TakeDamage(DamageAmount);	// float -> int
	}


	return DamageAmount;

}

void ANPCharacter::SetupMonsterWidget(UMyUserWidget* InUserWidget)
{



}

void ANPCharacter::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

void ANPCharacter::Debug()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, strState[m_CharacterState]);
}

