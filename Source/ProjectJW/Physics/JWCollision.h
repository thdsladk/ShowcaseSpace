
#pragma once

#include "CoreMinimal.h"

#define CPROFILE_JWCAPSULE TEXT("JWCapsule")
#define CPROFILE_JWTRIGGER TEXT("JWTrigger")


// Object Channel 
#define CCHANNEL_JWACTION		ECC_GameTraceChannel1
#define CCHANNEL_JWPLAYER		ECC_GameTraceChannel2
#define CCHANNEL_JWENEMY		ECC_GameTraceChannel3
#define CCHANNEL_JWITEM			ECC_GameTraceChannel4

// Trace Channel
#define CCHANNEL_JWPLAYERATTACK ECC_GameTraceChannel5
#define CCHANNEL_JWENEMYATTACK	ECC_GameTraceChannel6
#define CCHANNEL_JWSIGHT		ECC_GameTraceChannel7


UENUM(BlueprintType)
enum class EJWCollisionChannel : uint8
{
	ECC_WorldStatic UMETA(DisplayName = "WorldStatic"),
	ECC_WorldDynamic UMETA(DisplayName = "WorldDynamic"),
	ECC_Pawn UMETA(DisplayName = "Pawn"),
	ECC_Visibility UMETA(DisplayName = "Visibility", TraceQuery = "1"),
	ECC_Camera UMETA(DisplayName = "Camera", TraceQuery = "1"),
	ECC_PhysicsBody UMETA(DisplayName = "PhysicsBody"),
	ECC_Vehicle UMETA(DisplayName = "Vehicle"),
	ECC_Destructible UMETA(DisplayName = "Destructible"),

	/** Reserved for gizmo collision */
	ECC_EngineTraceChannel1		UMETA(Hidden),

	ECC_EngineTraceChannel2		UMETA(Hidden),
	ECC_EngineTraceChannel3		UMETA(Hidden),
	ECC_EngineTraceChannel4		UMETA(Hidden),
	ECC_EngineTraceChannel5		UMETA(Hidden),
	ECC_EngineTraceChannel6		UMETA(Hidden),

	JWAction					UMETA(DisplayName = "JWAction"),
	JWPlayer					UMETA(DisplayName = "JWPlayer"),
	JWEnemy						UMETA(DisplayName = "JWEnemy"),
	JWItem						UMETA(DisplayName = "JWItem"),
	JWPlayerAttack				UMETA(DisplayName = "JWPlayerAttack"),
	JWEnemyAttack				UMETA(DisplayName = "JWEnemyAttack"),
	JWSight						UMETA(DisplayName = "JWSight"),
	NoName_GameTraceChannel8	UMETA(Hidden),
	NoName_GameTraceChannel9	UMETA(Hidden),
	NoName_GameTraceChannel10	UMETA(Hidden),
	NoName_GameTraceChannel11	UMETA(Hidden),
	NoName_GameTraceChannel12	UMETA(Hidden),
	NoName_GameTraceChannel13	UMETA(Hidden),
	NoName_GameTraceChannel14	UMETA(Hidden),
	NoName_GameTraceChannel15	UMETA(Hidden),
	NoName_GameTraceChannel16	UMETA(Hidden),
	NoName_GameTraceChannel17	UMETA(Hidden),
	NoName_GameTraceChannel18	UMETA(Hidden),
    // 필요하면 계속 추가...
};


// Object Channels & Trace Channels 정의를 깔끔하게 using으로 정리
namespace CChannel
{
    using enum ECollisionChannel;

    // Object Channel
    constexpr ECollisionChannel JWAction = ECC_GameTraceChannel1;
    constexpr ECollisionChannel JWPlayer = ECC_GameTraceChannel2;
    constexpr ECollisionChannel JWEnemy = ECC_GameTraceChannel3;
    constexpr ECollisionChannel JWItem = ECC_GameTraceChannel4;

    // Trace Channel
    constexpr ECollisionChannel JWPlayerAttack = ECC_GameTraceChannel5;
    constexpr ECollisionChannel JWEnemyAttack = ECC_GameTraceChannel6;
    constexpr ECollisionChannel JWSight = ECC_GameTraceChannel7;
}