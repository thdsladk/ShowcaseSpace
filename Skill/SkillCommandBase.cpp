// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillCommandBase.h"

// Sets default values
ASkillCommandBase::ASkillCommandBase()
	:m_bCanEverTick(false), m_bEnableSkill(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
