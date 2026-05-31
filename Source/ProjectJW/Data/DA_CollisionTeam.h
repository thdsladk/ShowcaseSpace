// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_CollisionTeam.generated.h"

/**
 * CCHANNEL_JWACTION				== 14	
 * CCHANNEL_JWPLAYER				== 15	
 * CCHANNEL_JWENEMY					== 16
 * CCHANNEL_JWITEM					== 17
 * 
 * CCHANNEL_JWPLAYERATTACK			== 18
 * CCHANNEL_JWENEMYATTACK			== 19
 * CCHANNEL_JWSIGHT					== 20
 */

UENUM()
enum class ECollisionChannelWrapping : uint8
{
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
	Sight			UMETA(DisplayName = "Sight"),
	End

};


UCLASS()
class PROJECTJW_API UDA_CollisionTeam : public UPrimaryDataAsset
{
	GENERATED_BODY()
	// ECollisionChannel로 변경해서 써야할듯 하다.
public:
	UPROPERTY(EditAnywhere, Category = "Collision")
	ECollisionChannelWrapping FriendyCollision;
	UPROPERTY(EditAnywhere, Category = "Collision")
	ECollisionChannelWrapping NeutralCollision;
	UPROPERTY(EditAnywhere, Category = "Collision")
	ECollisionChannelWrapping HostileCollision;
};
