#pragma once 

#include "GameplayTagContainer.h"


#define JWTAG_ACTOR_ROTATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define JWTAG_ACTOR_ISROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.IsRotating"))

#define JWTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

// ##Begin Character State Tags
#define JWTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define JWTAG_CHARACTER_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))
#define JWTAG_CHARACTER_ISMOVE FGameplayTag::RequestGameplayTag(FName("Character.State.IsMove"))
#define JWTAG_CHARACTER_ISSTUN FGameplayTag::RequestGameplayTag(FName("Character.State.IsStun"))
#define JWTAG_CHARACTER_ALLOWPARRY FGameplayTag::RequestGameplayTag(FName("Character.State.AllowParry"))
#define JWTAG_CHARACTER_ISTARGETING FGameplayTag::RequestGameplayTag(FName("Character.State.IsTargeting"))
#define JWTAG_CHARACTER_ISDEFENSE FGameplayTag::RequestGameplayTag(FName("Character.State.IsDefense")) 
#define JWTAG_CHARACTER_ISLINKEDACTION FGameplayTag::RequestGameplayTag(FName("Character.State.IsLinkedAction"))
// ##End Character State Tags


#define JWTAG_NPC_ISSTRAFING FGameplayTag::RequestGameplayTag(FName("NPC.Status.IsStrafing"))
#define JWTAG_NPC_ISUNDERATTACK FGameplayTag::RequestGameplayTag(FName("NPC.Status.IsUnderAttack"))


#define JWTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))

#define JWTAG_EVENT_PLAYER_COMBATMODE_IDLE FGameplayTag::RequestGameplayTag(FName("Event.Player.ChangeCombatMode.Idle"))
#define JWTAG_EVENT_PLAYER_COMBATMODE_MELEE FGameplayTag::RequestGameplayTag(FName("Event.Player.ChangeCombatMode.Melee"))
#define JWTAG_EVENT_PLAYER_COMBATMODE_RANGE FGameplayTag::RequestGameplayTag(FName("Event.Player.ChangeCombatMode.Range"))
#define JWTAG_EVENT_PLAYER_COMBATMODE_DUALBLADE FGameplayTag::RequestGameplayTag(FName("Event.Player.ChangeCombatMode.DualBlade"))
#define JWTAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define JWTAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Unequip"))
#define JWTAG_EVENT_CHARACTER_HITPAUSE FGameplayTag::RequestGameplayTag(FName("Event.Character.HitPause"))
#define JWTAG_EVENT_CHARACTER_HITCHECK FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.HitCheck"))
#define JWTAG_EVENT_CHARACTER_SKILLHITCHECK FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.SkillHitCheck"))
#define JWTAG_EVENT_CHARACTER_STUN FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Stun"))
#define JWTAG_EVENT_CHARACTER_THUNDER FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Thunder"))
#define JWTAG_EVENT_CHARACTER_LIGTHHIT FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.LightHit"))
#define JWTAG_EVENT_CHARACTER_HEAVYHIT FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.HeavyHit"))
#define JWTAG_EVENT_CHARACTER_FLOAT FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Float"))
#define JWTAG_EVENT_CHARACTER_PLAYSKILL FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.PlaySkill"))
#define JWTAG_EVENT_CHARACTER_SKILL_ACTIVATED FGameplayTag::RequestGameplayTag(FName("Event.Skill.Activated"))

#define JWTAG_EVENT_CHARACTER_SKILL_FLOAT FGameplayTag::RequestGameplayTag(FName("Event.Skill.Float"))
#define JWTAG_EVENT_CHARACTER_SKILL_STUN FGameplayTag::RequestGameplayTag(FName("Event.Skill.Stun"))
#define JWTAG_EVENT_CHARACTER_SKILL_AROUND FGameplayTag::RequestGameplayTag(FName("Event.Skill.Around"))
#define JWTAG_EVENT_CHARACTER_SKILL_DASHATTACK FGameplayTag::RequestGameplayTag(FName("Event.Skill.DashAttack"))


#define JWTAG_CHARACTER_NATIVEHEAL FGameplayTag::RequestGameplayTag(FName("Character.Action.NativeHeal"))
#define JWTAG_CHARACTER_LINKEDACTION FGameplayTag::RequestGameplayTag(FName("Character.Action.LinkedAction"))
#define JWTAG_CHARACTER_ACTION_SKILLQ FGameplayTag::RequestGameplayTag(FName("Character.Action.SkillQ"))
#define JWTAG_CHARACTER_ACTION_SKILLE FGameplayTag::RequestGameplayTag(FName("Character.Action.SkillE")) 
#define JWTAG_CHARACTER_ACTION_SKILLR FGameplayTag::RequestGameplayTag(FName("Character.Action.SkillR")) 
#define JWTAG_CHARACTER_ACTION_SKILLX FGameplayTag::RequestGameplayTag(FName("Character.Action.SkillX")) 

#define JWTAG_CHARACTER_SKILL_AROUND FGameplayTag::RequestGameplayTag(FName("Character.Skill.Around"))
#define JWTAG_CHARACTER_SKILL_STUN FGameplayTag::RequestGameplayTag(FName("Character.Skill.Stun"))
#define JWTAG_CHARACTER_SKILL_FLOAT FGameplayTag::RequestGameplayTag(FName("Character.Skill.Float"))
#define JWTAG_CHARACTER_SKILL_DASHATTACK FGameplayTag::RequestGameplayTag(FName("Character.Skill.DashAttack"))

#define JWTAG_CHARACTER_BUFF_RANGEDCHARGE FGameplayTag::RequestGameplayTag(FName("Character.Buff.RangedCharge"))

#define JWTAG_CHARACTER_STATE_ISCONFIRMATION FGameplayTag::RequestGameplayTag(FName("Player.State.bIsSkillAwaitingConfirmation"))

#define JWTAG_PLAYER_IDLE FGameplayTag::RequestGameplayTag(FName("Player.CombatMode.Idle"))
#define JWTAG_PLAYER_MELEE FGameplayTag::RequestGameplayTag(FName("Player.CombatMode.Melee"))
#define JWTAG_PLAYER_RANGE FGameplayTag::RequestGameplayTag(FName("Player.CombatMode.Range"))
#define JWTAG_PLAYER_DUALBLADE FGameplayTag::RequestGameplayTag(FName("Player.CombatMode.DualBlade"))


#define JWTAG_PLAYER_WEAPON_SWORD FGameplayTag::RequestGameplayTag(FName("Player.Weapon.Sword"))
#define JWTAG_PLAYER_WEAPON_SHIELD FGameplayTag::RequestGameplayTag(FName("Player.Weapon.Shield"))
#define JWTAG_PLAYER_WEAPON_BOW FGameplayTag::RequestGameplayTag(FName("Player.Weapon.Bow"))
#define JWTAG_PLAYER_WEAPON_DUALBLADE_L FGameplayTag::RequestGameplayTag(FName("Player.Weapon.DualBladeL"))
#define JWTAG_PLAYER_WEAPON_DUALBLADE_R FGameplayTag::RequestGameplayTag(FName("Player.Weapon.DualBladeR"))



#define JWTAG_EVENT_WEAPON_SWORD FGameplayTag::RequestGameplayTag(FName("Event.Weapon.Sword"))
#define JWTAG_EVENT_WEAPON_SHIELD FGameplayTag::RequestGameplayTag(FName("Event.Weapon.Shield"))
#define JWTAG_EVENT_WEAPON_BOW FGameplayTag::RequestGameplayTag(FName("Event.Weapon.Bow"))
#define JWTAG_EVENT_WEAPON_DUALBLADE FGameplayTag::RequestGameplayTag(FName("Event.Weapon.DualBlade"))


#define JWTAG_SHARED_EVENT_MELEEDAMAGE FGameplayTag::RequestGameplayTag(FName("Shared.Event.MeleeDamage"))
#define JWTAG_SHARED_EVENT_RANGEDAMAGE FGameplayTag::RequestGameplayTag(FName("Shared.Event.RangeDamage"))
#define JWTAG_SHARED_EVENT_HITREACT FGameplayTag::RequestGameplayTag(FName("Shared.Event.HitReact"))

#define JWTAG_SHARED_SETBYCALLER_BASEDAMAGE FGameplayTag::RequestGameplayTag(FName("Shared.SetByCaller.BaseDamage"))
#define JWTAG_SHARED_ABILITY_HITREACT FGameplayTag::RequestGameplayTag(FName("Shared.Ability.HitReact"))


#define JWTAG_SKILL_CONFIRM_INSTANT FGameplayTag::RequestGameplayTag(FName("Skill.Confirm.Instant"))
#define JWTAG_SKILL_CONFIRM_USERCONFIRM FGameplayTag::RequestGameplayTag(FName("Skill.Confirm.UserConfirm"))




