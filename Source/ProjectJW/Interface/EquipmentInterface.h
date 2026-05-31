// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Header/GlobalEnum.h"
#include "EquipmentInterface.generated.h"

UENUM()
enum class EEquipmentSocketName : uint8
{
	HeadSocket			UMETA(DisplayName = "HeadSocket"),
	WeaponSocketL		UMETA(DisplayName = "WeaponSocketL"),
	ChestSocket			UMETA(DisplayName = "ChestSocket"),
	WeaponSocketR		UMETA(DisplayName = "WeaponSocketR"),
	FootSocketL			UMETA(DisplayName = "FootSocketL"),
	FootSocketR			UMETA(DisplayName = "FootSocketR"),
	End
};

USTRUCT()
struct FEquipmentFlags
{
	GENERATED_BODY()

	uint8 IsEquip_Head  : 1;
	uint8 IsEquip_HandL : 1;
	uint8 IsEquip_Chest : 1;
	uint8 IsEquip_HandR : 1;
	uint8 IsEquip_FootL : 1;
	uint8 IsEquip_FootR : 1;

    bool IsEquipped(EEquipmentSocketName Socket) const
    {
        switch (Socket)
        {
        case EEquipmentSocketName::HeadSocket:     return IsEquip_Head;
        case EEquipmentSocketName::WeaponSocketL:  return IsEquip_HandL;
        case EEquipmentSocketName::ChestSocket:    return IsEquip_Chest;
        case EEquipmentSocketName::WeaponSocketR:  return IsEquip_HandR;
        case EEquipmentSocketName::FootSocketL:    return IsEquip_FootL;
        case EEquipmentSocketName::FootSocketR:    return IsEquip_FootR;
        default: return false;
        }
    }

    void SetEquipped(EEquipmentSocketName Socket, bool bEquipped)
    {
        switch (Socket)
        {
        case EEquipmentSocketName::HeadSocket:     IsEquip_Head = bEquipped; break;
        case EEquipmentSocketName::WeaponSocketL:  IsEquip_HandL = bEquipped; break;
        case EEquipmentSocketName::ChestSocket:    IsEquip_Chest = bEquipped; break;
        case EEquipmentSocketName::WeaponSocketR:  IsEquip_HandR = bEquipped; break;
        case EEquipmentSocketName::FootSocketL:    IsEquip_FootL = bEquipped; break;
        case EEquipmentSocketName::FootSocketR:    IsEquip_FootR = bEquipped; break;
        default: break;
        }
    }

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTJW_API IEquipmentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Equip(EEquipmentSocketName SocketName, EEquipmentType Type = EEquipmentType::None) = 0;
	virtual void UnEquip(EEquipmentSocketName SocketName, EEquipmentType Type = EEquipmentType::None) = 0;

};
