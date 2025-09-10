// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyAbilityData.h"
#include "AbilityComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbillity, uint8);
//DECLARE_MULTICAST_DELEGATE_OneParam(FEndAbillity, uint8);

//DECLARE_MULTICAST_DELEGATE_TwoParams(FEndCoolDown,FGameplayTag,double);


class ACharacterBase;
class UAnimInstanceBase;
class UMyStatComponent;
class ASkillCommandBase;

class UMyAbilityInstance;

/*

*/

#pragma region Subjects Region 
USTRUCT(BlueprintType)
struct FAbilityHandle {
	GENERATED_BODY()

	UPROPERTY() 
	FGuid Id;
	bool IsValid() const { return Id.IsValid(); }
	static FAbilityHandle New() 
	{ 
		FAbilityHandle H; 
		H.Id = FGuid::NewGuid(); 
		return H; 
	}
};

UENUM(BlueprintType)
enum class EAbilityActivateResult : uint8 {
	Success,
	NotGranted, 
	BlockedTags, 
	MissingTags, 
	OnCooldown, 
	InsufficientCost, 
	AlreadyActive, 
	InternalError
};

#pragma endregion


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Grant & revoke
	UFUNCTION(BlueprintCallable) 
	void GrantAbility(UMyAbilityData* Data);
	UFUNCTION(BlueprintCallable) 
	void RevokeByTag(FGameplayTag AbilityTag);

	// Activate & cancel
	UFUNCTION(BlueprintCallable) 
	EAbilityActivateResult TryActivateByTag(FGameplayTag AbilityTag, FAbilityHandle& OutHandle);
	UFUNCTION(BlueprintCallable) 
	bool CancelByHandle(FAbilityHandle Handle /*,FGameplayTag Reason*/);

	// Query
	//UFUNCTION(BlueprintPure) 
	bool IsOnCooldown(FGameplayTag CooldownTag) const;
	//UFUNCTION(BlueprintPure)
	double GetCooldownRatio(FGameplayTag CooldownTag) const;
	//UFUNCTION(BlueprintPure) 
	bool IsActive(FAbilityHandle Handle) const;
	//UFUNCTION(BlueprintPure) 
	void GetActiveByTag(FGameplayTag AbilityTag, TArray<FAbilityHandle>& Out) const;

	// Tags
	UFUNCTION(BlueprintCallable) 
	void AddOwnedTags(const FGameplayTagContainer& Tags);
	UFUNCTION(BlueprintCallable) 
	void RemoveOwnedTags(const FGameplayTagContainer& Tags);
	UFUNCTION(BlueprintCallable)
	bool CheckOwnedTags(const FGameplayTagContainer& Tags);


// 내부용 함수
private:
	FGameplayTag CutStringToTag(FGameplayTag& FullTag);

private:
	UPROPERTY() 
	TMap<FGameplayTag, TObjectPtr<UMyAbilityData>> m_Granted;
	UPROPERTY() 
	TMap<FGuid, TObjectPtr<UMyAbilityInstance>> m_Actives;
	UPROPERTY() 
	TMap<FGameplayTag, double> m_CooldownsUTC;
	UPROPERTY() 
	FGameplayTagContainer m_OwnedTags;

};
