// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon/GASWeaponBase.h"
#include "Header/GASStructTypes.h"
#include "GASPlayerWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API AGASPlayerWeaponBase : public AGASWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FPlayerWeaponData m_PlayerWeaponData;

};
