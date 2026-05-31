// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 *	 장비 관련된 열거형을 정의하는 헤더 
 */



UENUM()
enum class EEquipmentType : uint8
{
	None			UMETA(DisplayName = "None"),
	Sword			UMETA(DisplayName = "Sword"),
	Shield			UMETA(DisplayName = "Shield"),
	Bow				UMETA(DisplayName = "Bow"),
	DualBladeL		UMETA(DisplayName = "DualBladeL"),
	DualBladeR		UMETA(DisplayName = "DualBladeR"),
	End
};
