// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterComponents/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "Header/GlobalEnum.h"
#include "PawnCombatComponent.generated.h"

class AGASWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Pawn|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AGASWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Pawn|Combat")
	AGASWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	//UPROPERTY(BlueprintReadWrite, Category = "Pawn|Combat")
	//FGameplayTag m_CurrentEquippedWeaponTag;

	UPROPERTY(BlueprintReadWrite, Category = "Pawn|Combat")
	TMap<EEquipmentType,FGameplayTag> m_CurrentEquippedWeaponTags;


	UFUNCTION(BlueprintCallable, Category = "Pawn|Combat")
	AGASWeaponBase* GetCharacterCurrentEquippedWeapon(EEquipmentType Type) const;

	UFUNCTION(BlueprintCallable, Category = "Pawn|Combat")
	TArray<AGASWeaponBase*> GetCharacterAllCurrentEquippedWeapons() const;


	UFUNCTION(BlueprintCallable, Category = "Pawn|Combat")
	void AddCurrentEquippedWeapon(EEquipmentType Type, FGameplayTag InWeaponTagToRegister);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);


protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);


protected:
	TArray<AActor*> m_OverlappedActors;
private:
	TMap<FGameplayTag, AGASWeaponBase*> m_CharacterCarriedWeaponMap;
};
