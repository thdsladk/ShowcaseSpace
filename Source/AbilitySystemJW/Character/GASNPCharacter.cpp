// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASNPCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"

#include "Attribute/CharacterAttributeSet.h"
#include "CharacterComponents/GASAIController.h"
#include "Data/DA_StartupNPCharacter.h"
#include "CharacterComponents/PawnCombatComponent.h"
#include "CharacterComponents/TargetSystemComponent.h"
#include "CharacterComponents/PhysicsMotionSystemComponent.h"
#include "UI/GASWidgetComponent.h"
#include "UI/GASGaugeBarUserWidget.h"
#include "Tag/JWGameplayTag.h"
#include "UI/DamageFontWidget.h"
#include "Kismet/GameplayStatics.h"



AGASNPCharacter::AGASNPCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AIControllerClass = AGASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	
	
	m_AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	m_AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));


#pragma region Widget Setting

	// 아마 이름을 Widget으로만 해야할수도 있다.... [ 기본으로 설정되는 이름만 가능할수도 ]  
	m_GaugeBar = CreateDefaultSubobject<UGASWidgetComponent>(TEXT("BaseGaugeWidget"));
	m_GaugeBar->SetupAttachment(GetMesh());
	m_GaugeBar->SetRelativeLocation(FVector(0.0f, 0.0f, 240.0f));

	

#pragma endregion


	m_Level = 1;

	m_PawnCombatComponent = CreateDefaultSubobject<UPawnCombatComponent>(TEXT("PawnCombatComponent"));
	m_TargetSystemComponent = CreateDefaultSubobject<UTargetSystemComponent>(TEXT("TargetSystemComponent"));
	m_PhysicsMotionSystemComponent = CreateDefaultSubobject<UPhysicsMotionSystemComponent>(TEXT("PhysicsMotionSystemComponent"));

}

UAbilitySystemComponent* AGASNPCharacter::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

void AGASNPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	m_AbilitySystemComponent->InitAbilityActorInfo(this, this);
	m_AttributeSet->OnOutOfHealth.AddDynamic(this, &AGASNPCharacter::OnOutOfHealth);

	if(m_AbilitySystemComponent != nullptr )
	{
		if (UDA_StartupNPCharacter* StartupData = m_StartupNPCharacterData.LoadSynchronous())
		{
			// Start Ability Setting
			for(const auto& StartAbility : StartupData->StartAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartAbility);
				m_AbilitySystemComponent->GiveAbility(StartSpec);
			}

			// Passive Ability Setting		
			for (const auto& PassiveAbility : StartupData->PassiveAbilities)
			{
				FGameplayAbilitySpec PassiveSpec(PassiveAbility);
				m_AbilitySystemComponent->GiveAbility(PassiveAbility);
			}

			// Play Effect
			if (StartupData->StartupEffects.IsEmpty() == false)
			{
				for (const auto& StartupEffect : StartupData->StartupEffects)
				{
					UGameplayEffect* EffectCDO = StartupEffect->GetDefaultObject<UGameplayEffect>();
					m_AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectCDO, m_Level, m_AbilitySystemComponent->MakeEffectContext());
				}
			}
		}
	}


	FGameplayEffectContextHandle EffectContextHandle = m_AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = m_AbilitySystemComponent->MakeOutgoingSpec(m_InitStatEffect, m_Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		m_AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AGASNPCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (m_AbilitySystemComponent != nullptr)
	{
		if (m_StartupNPCharacterData.IsValid() == true)
		{
			for (const auto& PassiveAbility : m_StartupNPCharacterData.Get()->PassiveAbilities)
			{
				m_AbilitySystemComponent->TryActivateAbilityByClass(PassiveAbility);
			}
		}

	}


	if (m_GaugeWidgetClass != nullptr && m_GaugeBar != nullptr)
	{
		m_GaugeBar->SetWidgetClass(m_GaugeWidgetClass);
		m_GaugeBar->SetWidgetSpace(EWidgetSpace::Screen);
		// 세팅을 강제 하기도 하지만 블루프린트에서 세팅해주는게 나을 것같다. 
		//m_GaugeBar->SetDrawSize(m_GaugeBarToolSize);//FVector2D(100.0f, 20.f));
		m_GaugeBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

		// Init Gauge Widget
		m_GaugeBar->InitWidget();
		m_GaugeBar->SetDrawSize(m_GaugeBarToolSize);

		m_GaugeBar->SetVisibility(false);

	}


}
/*
void AGASNPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// 디버그 용으로 임시로 틱을 쓰고 있다.
	



}
*/
void AGASNPCharacter::OnOutOfHealth()
{
	if (m_AbilitySystemComponent != nullptr)
	{
		m_AbilitySystemComponent->AddLooseGameplayTag(JWTAG_CHARACTER_ISDEAD);
	}

	if (m_GaugeBar != nullptr)
	{
		m_GaugeBar->SetActive(false);
	}
	
	CastChecked<AGASAIController>(GetController())->StopAI();

	SetDead();
}

void AGASNPCharacter::TryActivateAbility_Tag(FGameplayTag Tag)
{
	if (m_AbilitySystemComponent != nullptr)
	{

		// 태그 컨테이너 생성
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(Tag);

		// 어빌리티 시스템에 태그가 있는지 확인은 하지 말고 Default Ability Tag를 통해서 해야하니까 
		// 바로 실행해서 맞는 어빌리티를 실행하도록 하자 
		// HasMatchingGameplayTag는 이미 활성화 된 태그를 기준으로 실행시키는 방식이다. 
		//if (m_AbilitySystemComponent->HasMatchingGameplayTag(Tag))
		{
			// 태그로 어빌리티 실행 시도
			m_AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		}

	}

}

void AGASNPCharacter::TryActivateAbility_Class(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (m_AbilitySystemComponent != nullptr)
	{
		for (const FGameplayAbilitySpec& Spec : m_AbilitySystemComponent->GetActivatableAbilities())
		{
			if (AbilityClass && Spec.Ability.GetClass() == AbilityClass)
			{
				m_AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass);
			}
		}
	}
}

void AGASNPCharacter::SetVisibleGaugeBar_Implementation(bool bVisible)
{
	if (m_GaugeBar != nullptr)
	{
		m_GaugeBar->SetVisibility(bVisible);
	}
}

void AGASNPCharacter::PlayDamageFont(float DamageAmount, FVector HitPoint)
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
