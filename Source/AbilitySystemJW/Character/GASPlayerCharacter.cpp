// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Player/JWGASPlayerState.h"
#include "UI/GASWidgetComponent.h"
#include "AbilitySystem/Attribute/CharacterAttributeSet.h"
#include "Data/DA_StartupCharacter.h"
#include "Data/DA_StartupCharacterSkill.h"
#include "Interface/PlayerControllerInterface.h"
#include "Components/PlayerCombatComponent.h"
#include "Components/TargetSystemComponent.h"
#include "Components/PhysicsMotionSystemComponent.h"
#include "Components/GASDecalComponent.h"
#include "Components/LinkedActionComponent.h"
#include "Items/Weapon/GASWeaponBase.h"
#include "UI/DamageFontWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/AnimNotifyInterface.h"
#include "Interface/PlayerControllerInterface.h"

#include "Header\JWGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"

//#include "AbilitySystemJW.h"
//#include "MotionWarpingComponent.h"
//#include "Character\GASPlayerController.h"

AGASPlayerCharacter::AGASPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,m_GaugeBarToolSize(100.f,20.f)
{
	// 어빌리티를 저장해서 관리하기 위한 리스트 초기화.
	m_AbilitySystemComponent = nullptr;
	

#pragma region Widget Setting

	// 플레이어 게이지는 HUD로 이전.
	//m_GaugeBar = CreateDefaultSubobject<UGASWidgetComponent>(TEXT("GaugeWidget"));
	//m_GaugeBar->SetupAttachment(GetMesh());
	//m_GaugeBar->SetRelativeLocation(FVector(0.0f, 0.0f, 280.0f)); 
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/JWProject/UI/WBP_GaugeBar.WBP_GaugeBar_C"));
	//if (HpBarWidgetRef.Class) 
	//{
	//	m_GaugeBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	m_GaugeBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	//m_GaugeBar->SetDrawSize(m_GaugeBarToolSize);
	//	m_GaugeBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}


	m_RangeGaugeBar = CreateDefaultSubobject<UGASWidgetComponent>(TEXT("RangeGaugeWidget"));
	m_RangeGaugeBar->SetupAttachment(GetMesh());
	m_RangeGaugeBar->SetRelativeLocation(FVector(0.0f, 0.0f, 280.0f)); //280.f
	static ConstructorHelpers::FClassFinder<UUserWidget> RangeBarWidgetRef(TEXT("/Game/JWProject/UI/WBP_ArrowBar.WBP_ArrowBar_C")); 
	if (RangeBarWidgetRef.Class)
	{
		m_RangeGaugeBar->SetWidgetClass(RangeBarWidgetRef.Class);
		m_RangeGaugeBar->SetWidgetSpace(EWidgetSpace::Screen);
		// 세팅을 강제 하기도 하지만 블루프린트에서 세팅해주는게 나을 것같다. 
		m_RangeGaugeBar->SetDrawSize(FVector2D(100.0f, 20.f));
		m_RangeGaugeBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_RangeGaugeBar->SetVisibility(false);
	}



#pragma endregion

	// 소켓이름들 정의
	m_EquipmentSocketNames.Add(EEquipmentSocketName::HeadSocket, FName("HeadSocket"));
	m_EquipmentSocketNames.Add(EEquipmentSocketName::WeaponSocketL, FName("WeaponSocketL"));
	m_EquipmentSocketNames.Add(EEquipmentSocketName::ChestSocket, FName("ChestSocket"));
	m_EquipmentSocketNames.Add(EEquipmentSocketName::WeaponSocketR, FName("WeaponSocketR"));
	m_EquipmentSocketNames.Add(EEquipmentSocketName::FootSocketL, FName("FootSocketL"));
	m_EquipmentSocketNames.Add(EEquipmentSocketName::FootSocketR, FName("FootSocketR"));

	m_EquipmentBackSocketNames.Add(EEquipmentType::Sword, FName("BackSocket_Sword"));
	m_EquipmentBackSocketNames.Add(EEquipmentType::Shield, FName("BackSocket_Shield"));
	m_EquipmentBackSocketNames.Add(EEquipmentType::Bow, FName("BackSocket_Bow"));
	m_EquipmentBackSocketNames.Add(EEquipmentType::DualBladeL, FName("BackSocket_DualBladeL"));
	m_EquipmentBackSocketNames.Add(EEquipmentType::DualBladeR, FName("BackSocket_DualBladeR"));

	
	m_PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	m_TargetSystemComponent = CreateDefaultSubobject<UTargetSystemComponent>(TEXT("TargetSystemComponent"));
	m_PhysicsMotionSystemComponent = CreateDefaultSubobject<UPhysicsMotionSystemComponent>(TEXT("PhysicsMotionSystemComponent"));
	m_GASDecalComponent = CreateDefaultSubobject<UGASDecalComponent>(TEXT("GASDecalComponent"));
	m_LinkedActionComponent = CreateDefaultSubobject<ULinkedActionComponent>(TEXT("LinkedActionComponent"));

	m_PlayerCombatComponent->ComponentTags.Add(TEXT("PlayerCombatComponent"));
	m_TargetSystemComponent->ComponentTags.Add(TEXT("TargetSystemComponent"));
	m_PhysicsMotionSystemComponent->ComponentTags.Add(TEXT("PhysicsMotionSystemComponent"));
	m_GASDecalComponent->ComponentTags.Add(TEXT("GASDecalComponent"));
	m_LinkedActionComponent->ComponentTags.Add(TEXT("LinkedActionComponent"));


	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hellhammer/SK_Blunt_HellHammer.SK_Blunt_HellHammer'"));
	//if (WeaponMeshRef.Object)
	//{
	//	WeaponMesh = WeaponMeshRef.Object;
	//}
	//
	//WeaponRange = 75.f;
	//WeaponAttackRate = 100.0f;
	//
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> SKillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattleGAS/Animation/AM_SkillAttack.AM_SkillAttack'"));
	//if (SKillActionMontageRef.Object)
	//{
	//	SkillActionMontage = SKillActionMontageRef.Object;
	//}
}

UAbilitySystemComponent* AGASPlayerCharacter::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

void AGASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AJWGASPlayerState* GASPS = GetPlayerState<AJWGASPlayerState>();
	if (GASPS != nullptr)
	{
		m_AbilitySystemComponent = GASPS->GetAbilitySystemComponent();
		
		if(m_AbilitySystemComponent != nullptr)
		{

			m_AbilitySystemComponent->InitAbilityActorInfo(GASPS, this);

			//m_AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AABGASCharacterPlayer::EquipWeapon);
			//m_AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AABGASCharacterPlayer::UnequipWeapon);

			IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(NewController);

			const UCharacterAttributeSet* CurrentAttributeSet = m_AbilitySystemComponent->GetSet<UCharacterAttributeSet>();
			if (CurrentAttributeSet != nullptr)
			{
				CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
			}

			// 에셋데이터를 받아와서 세팅해주는 방식 
			if (m_StartupCharacterData != nullptr || m_StartupCharacterData.IsNull() == false)
			{
				m_StartupCharacterDataObject = m_StartupCharacterData.LoadSynchronous();

				if (m_StartupCharacterSkillDataObject.Contains(ECombatMode(PCI->GetCombatMode())) == false)
				{
					m_StartupCharacterSkillDataObject.Add(ECombatMode(PCI->GetCombatMode()), m_StartupCharacterSkillData[ECombatMode(PCI->GetCombatMode())].LoadSynchronous());
				}
			}

			
			for (const auto& StartAbility : m_StartupCharacterDataObject->StartAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartAbility);
				m_AbilitySystemComponent->GiveAbility(StartSpec);
			}

			for (const auto& StartInputAbility : m_StartupCharacterDataObject->StartInputAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
				StartSpec.InputID = int32(StartInputAbility.Key);
				m_AbilitySystemComponent->GiveAbility(StartSpec);
			}

			// Skill Ability Grant
			GrantSkillAbility(PCI->GetCombatMode());

			// Play Effect
			if (m_StartupCharacterDataObject->StartupEffects.IsEmpty() == false)
			{
				for (const auto& StartupEffect : m_StartupCharacterDataObject->StartupEffects)
				{
					UGameplayEffect* EffectCDO =  StartupEffect->GetDefaultObject<UGameplayEffect>();
					m_AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectCDO,1.f, m_AbilitySystemComponent->MakeEffectContext());
				}
			}


			APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
			PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));


			// Initialize Passive Tag
			{
				m_AbilitySystemComponent->AddLooseGameplayTag(JWTAG_PLAYER_IDLE);

			}



		}
	}
}

#pragma region Movement Interface
void AGASPlayerCharacter::ShoulderMove(const FVector2D& Value)
{
	Super::ShoulderMove(Value);
	if(m_AbilitySystemComponent->HasMatchingGameplayTag(JWTAG_CHARACTER_ISMOVE) == false)
		m_AbilitySystemComponent->AddLooseGameplayTag(JWTAG_CHARACTER_ISMOVE);
}

void AGASPlayerCharacter::ShoulderLook(const FVector2D& Value)
{
	Super::ShoulderLook(Value);

}

void AGASPlayerCharacter::QuaterMove(FVector2D Value)
{
	Super::QuaterMove(Value);
	if (m_AbilitySystemComponent->HasMatchingGameplayTag(JWTAG_CHARACTER_ISMOVE) == false)
		m_AbilitySystemComponent->AddLooseGameplayTag(JWTAG_CHARACTER_ISMOVE);
}
void AGASPlayerCharacter::ReleaseMove(const FVector2D Value)
{
	Super::ReleaseMove(Value);
	if (m_AbilitySystemComponent->HasMatchingGameplayTag(JWTAG_CHARACTER_ISMOVE) == true)
		m_AbilitySystemComponent->RemoveLooseGameplayTag(JWTAG_CHARACTER_ISMOVE);
}
#pragma endregion

void AGASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGASPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AGASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Init Gauge Widget
	//m_GaugeBar->InitWidget();
	//m_GaugeBar->SetDrawSize(m_GaugeBarToolSize);


}

void AGASPlayerCharacter::GASInputPressed(int32 InputID)
{
	if (InputID == 1 || InputID == 6)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,JWTAG_EVENT_CHARACTER_PLAYSKILL, FGameplayEventData());
	}

	FGameplayAbilitySpec* Spec = m_AbilitySystemComponent->FindAbilitySpecFromInputID(InputID);
	if (Spec != nullptr)
	{
		Spec->InputPressed = true;
		Spec->InputID = InputID;
		
		// 어빌리티 활성화 처리
		if (Spec->IsActive() == true)
		{
			m_AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			if (m_AbilitySystemComponent->TryActivateAbility(Spec->Handle) == false)
			{
				// 어빌리티 활성화 실패시.
				if (Spec->Ability->AbilityTags.HasTagExact(JWTAG_CHARACTER_LINKEDACTION) == true)
				{
					m_LinkedActionComponent->AddAction(InputID);
				}
			}
		}

	}
}

void AGASPlayerCharacter::GASInputReleased(int32 InputID)
{
	FGameplayAbilitySpec* Spec = m_AbilitySystemComponent->FindAbilitySpecFromInputID(InputID);
	if (Spec != nullptr)
	{
		Spec->InputPressed = false;
		Spec->InputID = InputID;
		
		if (Spec->IsActive() == true)
		{
			m_AbilitySystemComponent->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AGASPlayerCharacter::GASInputConfirm(int32 InputID)
{
	// 타겟팅 중 태그를 하나 만들어서 그상황에서만 실행되도록 수정 필요 
	if (m_AbilitySystemComponent != nullptr)
	{
		m_AbilitySystemComponent->LocalInputConfirm();
	}
}

void AGASPlayerCharacter::GASInputCancel(int32 InputID)
{
	// 타겟팅 중 태그를 하나 만들어서 그상황에서만 실행되도록 수정 필요 
	if (m_AbilitySystemComponent != nullptr)
	{
		m_AbilitySystemComponent->LocalInputCancel();
	}
}

/*
void AGASPlayerCharacter::GASInputQuateMove(const FVector2D& Value, int32 InputID)
{
	if (Value.IsNearlyZero() == true)
		return;

	// 첫번째 탭 일 때
	if (m_bDoubleTab == false)
	{
		m_preDirection = Value.GetSafeNormal();
		m_bDoubleTab = true;

		// 시간안에 두번째 탭이 안들어오면 false로 변경
		GetWorld()->GetTimerManager().SetTimer(m_DoubleTabTimer, [&]()
			{
				m_bDoubleTab = false;
				m_preDirection = FVector2D::ZeroVector;
			}, m_DoubleTabLimit, false);
		
	}
	// 두번째 탭 일 때 
	else 
	{
		// 시간안에 들어오면 타이머 클리어 해줘야한다. 
		GetWorld()->GetTimerManager().ClearTimer(m_DoubleTabTimer);
		m_bDoubleTab = false;

		FVector2D CurrentDirection = Value.GetSafeNormal();
		if ((CurrentDirection - m_preDirection).IsNearlyZero() == true)
		{
			FGameplayAbilitySpec* Spec = m_AbilitySystemComponent->FindAbilitySpecFromInputID(InputID);
			if (Spec != nullptr)
			{
				Spec->InputPressed = true;
				if (Spec->IsActive() == true)
				{
					m_AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
				}
				else
				{
					m_AbilitySystemComponent->TryActivateAbility(Spec->Handle);
				}
			}
		}

	}

}
*/
void AGASPlayerCharacter::OnOutOfHealth()
{
	// 어트리뷰트에서 이미 부여하고 있어서 제거한다.
	//if (m_AbilitySystemComponent != nullptr)
	//{
	//	m_AbilitySystemComponent->AddLooseGameplayTag(JWTAG_CHARACTER_ISDEAD);
	//}
	// AnimInstance의 델리게이트를 바인딩하기
	CastChecked<IAnimNotifyInterface>(GetMesh()->GetAnimInstance())->GetDeathPoint().AddUObject(this, &AGASPlayerCharacter::DeathEnded);

	SetDead();
}

void AGASPlayerCharacter::DeathEnded()
{
	// 여기서 Death 모션이 끝나고 일어날 일들을 짜주면 된다. 
	CastChecked<IPlayerControllerInterface>(GetController())->ToggleDeathWidget();

}

#pragma region Equipment Interface
void AGASPlayerCharacter::Equip(EEquipmentSocketName SocketName, EEquipmentType Type)
{
	ECombatMode CurrentCombatMode = ECombatMode(CastChecked<IPlayerControllerInterface>(GetController())->GetCombatMode());
	FString TargetSocket = m_EquipmentSocketNames[SocketName].ToString();
	FGameplayTag EquipmentTag;
	// (임시) 하드코딩으로 되어 있어서 수정이 필요하다 . 
	if (CurrentCombatMode == ECombatMode::IdleMode)
	{
		TargetSocket += FString(TEXT("_Idle"));
		// (임시) 비무장은 장비 기능이 아직 없어서 패스하도록 한다. 
		return;
	}
	else if (CurrentCombatMode == ECombatMode::MeleeMode)
	{
		TargetSocket += FString(TEXT("_Melee"));
		if(SocketName == EEquipmentSocketName::WeaponSocketR)
		{
			Type = EEquipmentType::Sword;		
			EquipmentTag = JWTAG_PLAYER_WEAPON_SWORD;
		}
		else if (SocketName == EEquipmentSocketName::WeaponSocketL)
		{
			Type = EEquipmentType::Shield;
			EquipmentTag = JWTAG_PLAYER_WEAPON_SHIELD;
		}
	}
	else if (CurrentCombatMode == ECombatMode::RangeMode)
	{
		TargetSocket += FString(TEXT("_Range"));
		if (SocketName == EEquipmentSocketName::WeaponSocketL)
		{
			Type = EEquipmentType::Bow;
			EquipmentTag = JWTAG_PLAYER_WEAPON_BOW;
		}
	}
	else if (CurrentCombatMode == ECombatMode::DualBladeMode)
	{
		TargetSocket += FString(TEXT("_DualBlade"));
		if (SocketName == EEquipmentSocketName::WeaponSocketR)
		{
			Type = EEquipmentType::DualBladeR;
			EquipmentTag = JWTAG_PLAYER_WEAPON_DUALBLADE_R;
		}
		else if (SocketName == EEquipmentSocketName::WeaponSocketL)
		{
			Type = EEquipmentType::DualBladeL;
			EquipmentTag = JWTAG_PLAYER_WEAPON_DUALBLADE_L;
		}
	}

	if (Type == EEquipmentType::None)
	{
		return;

	}

	USkeletalMeshComponent* SkelComp = GetMesh();
	for (USceneComponent* ChildComp : SkelComp->GetAttachChildren())
	{
		if (ChildComp->GetAttachSocketName() == m_EquipmentBackSocketNames[Type])
		{
			// Detach
			ChildComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

			// Attach to new socket
			ChildComp->AttachToComponent(SkelComp, FAttachmentTransformRules::SnapToTargetIncludingScale, *TargetSocket);

			break;
		}
	}
	
	//(임시) 여기서 현재 무기를 세팅해주는게 정확히 맞는 타이밍인지 한번 더 고려해보자.
	m_PlayerCombatComponent->AddCurrentEquippedWeapon(Type, EquipmentTag);

}

void AGASPlayerCharacter::UnEquip(EEquipmentSocketName SocketName, EEquipmentType Type)
{

	USkeletalMeshComponent* SkelComp = GetMesh();

	ECombatMode CurrentCombatMode = ECombatMode(CastChecked<IPlayerControllerInterface>(GetController())->GetCombatMode());
	FString SourceSocket = m_EquipmentSocketNames[SocketName].ToString();

	// (임시) 하드코딩으로 되어 있어서 수정이 필요하다 . 
	if (CurrentCombatMode == ECombatMode::IdleMode)
	{
		SourceSocket += FString(TEXT("_Idle"));
		// (임시) 비무장은 장비 기능이 아직 없어서 패스하도록 한다. 
		return;
	}
	else if (CurrentCombatMode == ECombatMode::MeleeMode)
	{
		SourceSocket += FString(TEXT("_Melee"));
	}
	else if (CurrentCombatMode == ECombatMode::RangeMode)
	{
		SourceSocket += FString(TEXT("_Range"));
	}
	else if (CurrentCombatMode == ECombatMode::DualBladeMode)
	{
		SourceSocket += FString(TEXT("_DualBlade"));
	}


	for (USceneComponent* ChildComp : SkelComp->GetAttachChildren())
	{
		if (ChildComp->GetAttachSocketName() == SourceSocket)
		{
			Type = EEquipmentType(CastChecked<AGASWeaponBase>(ChildComp->GetOwner())->GetEquipmentType());
			if(Type == EEquipmentType::None)
			{
				return;
			}


			FName TargetSocket = m_EquipmentBackSocketNames[Type];
			// Detach
			ChildComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

			// Attach to new socket
			ChildComp->AttachToComponent(SkelComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TargetSocket);


			break;
		}
	}

	//m_EquipmentMeshComponents[SocketName]->SetSkeletalMesh(nullptr);
	//m_EquipmentMeshComponents[SocketName]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//m_EquipmentMeshComponents[SocketName]->SetHiddenInGame(true);
	//m_IsEquippedList.SetEquipped(SocketName, false);
}

#pragma endregion

void AGASPlayerCharacter::PlayDamageFont(float DamageAmount, FVector HitPoint)
{
	// Spawn DamageFont				// 데미지 폰트가 먼저 세팅이 되어야 그다음에 데미지가 표현될거같다.
	if (m_DamageFontClass != nullptr)
	{
		// Socket 오버헤드가 존재해야한다. 
		// 0번째 플레이어 컨트롤러에 붙이고 있다. 
		FVector2D HitPoint2D;
		//FVector OverHead = CastChecked<ACharacter>(HitResult.GetActor())->GetMesh()->GetSocketLocation(TEXT("OverHead"));
		if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), HitPoint, HitPoint2D))
		{
			HitPoint2D.X += FMath::RandRange(-10.f, 10.f);
			HitPoint2D.Y += FMath::RandRange(-10.f, 10.f);

			UDamageFontWidget* DF = CreateWidget<UDamageFontWidget>(GetWorld(), m_DamageFontClass);
			if (DF != nullptr)
			{
				DF->SetPositionInViewport(HitPoint2D, true);
				DF->AddToViewport();

				DF->InitializeDamageFont(DamageAmount); // Miss 처리
				DF->SetAbilitySystemComponent(this);
				DF->PlayDamageFont();

			}
		}
		// 필요하다면 위치나 데미지 값 전달
		// 예: 캐릭터 위치에 따라 화면 좌표 변환 후 표시

	}
}

#pragma region GameplayTag Function
const FGameplayTag AGASPlayerCharacter::GetCombatModeTag() const
{
	if (m_AbilitySystemComponent != nullptr)
	{
		FGameplayTagContainer ActiveTags;
		TArray<FGameplayTag> MatchedTags;
		m_AbilitySystemComponent->GetOwnedGameplayTags(ActiveTags);

		for (const FGameplayTag& Tag : ActiveTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Player.CombatMode"))) == true) // 부모 태그와 계층적으로 일치하는지 검사
			{
				return Tag;
			}
		}
	}
	return FGameplayTag::EmptyTag;
}
#pragma endregion

#pragma region PawnCombat Interface
UPawnCombatComponent* AGASPlayerCharacter::GetPawnCombatComponent() const
{
	return m_PlayerCombatComponent;
}
#pragma endregion

#pragma region Ability Management Interface
void AGASPlayerCharacter::GrantSkillAbility(uint8 CombatMode)
{
	if (m_AbilitySystemComponent == nullptr) return;
	if (m_StartupCharacterSkillDataObject.Contains(ECombatMode(CombatMode)) == false)
	{
		m_StartupCharacterSkillDataObject.Add(ECombatMode(CombatMode), m_StartupCharacterSkillData[ECombatMode(CombatMode)].LoadSynchronous());
	}

	// Grant Ability
	for (const auto& StartSkillAbility : m_StartupCharacterSkillDataObject[ECombatMode(CombatMode)]->StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartSkillAbility);
		m_CurrentSkillAbilityList.SkillAbilitySpecHandles.Add(m_AbilitySystemComponent->GiveAbility(StartSpec));

	}
	for (const auto& StartSkillInputAbility : m_StartupCharacterSkillDataObject[ECombatMode(CombatMode)]->StartInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartSkillInputAbility.Value);
		StartSpec.InputID = int32(StartSkillInputAbility.Key);
		m_CurrentSkillAbilityList.InputSkillAbilitySpecHandles.Add(m_AbilitySystemComponent->GiveAbility(StartSpec));
	}

	// Grant Effect
	if (m_StartupCharacterSkillDataObject[ECombatMode(CombatMode)]->StartupEffects.IsEmpty() == false)
	{
		for (const auto& SkillEffect : m_StartupCharacterSkillDataObject[ECombatMode(CombatMode)]->StartupEffects)
		{
			UGameplayEffect* EffectCDO = SkillEffect->GetDefaultObject<UGameplayEffect>();
			m_CurrentSkillAbilityList.StartupEffectSpecHandles.Add(m_AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectCDO, 1.f, m_AbilitySystemComponent->MakeEffectContext()));
		}
	}


}

void AGASPlayerCharacter::RemoveSkillAbility()
{
	if (m_AbilitySystemComponent == nullptr) return;

	TArray<FGameplayAbilitySpecHandle>& SkillAbilitySpecHandles = m_CurrentSkillAbilityList.SkillAbilitySpecHandles;
	for (const FGameplayAbilitySpecHandle& Handle : SkillAbilitySpecHandles)
	{
		m_AbilitySystemComponent->ClearAbility(Handle);
	}
	TArray<FGameplayAbilitySpecHandle>& InputSkillAbilitySpecHandles = m_CurrentSkillAbilityList.InputSkillAbilitySpecHandles;
	for (const FGameplayAbilitySpecHandle& Handle : InputSkillAbilitySpecHandles)
	{
		m_AbilitySystemComponent->ClearAbility(Handle);
	}
	TArray<FActiveGameplayEffectHandle>& EffectSpecHandles = m_CurrentSkillAbilityList.StartupEffectSpecHandles;
	for (const FActiveGameplayEffectHandle& Handle : EffectSpecHandles)
	{
		m_AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
	}

}

void AGASPlayerCharacter::ChangeSkillAbility(uint8 CombatMode)
{
	RemoveSkillAbility();
	GrantSkillAbility(CombatMode);
}
#pragma endregion

void AGASPlayerCharacter::SetRangeGaugeBar_Implementation(bool bVisible)
{
	 m_RangeGaugeBar->SetVisibility(bVisible); 
}


