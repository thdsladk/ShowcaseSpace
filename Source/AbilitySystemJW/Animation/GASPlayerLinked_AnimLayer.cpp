// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GASPlayerLinked_AnimLayer.h"

UGASAnimInstanceBase* UGASPlayerLinked_AnimLayer::GetGASAnimInstanceBase() const
{
	return Cast<UGASAnimInstanceBase>(GetOwningComponent()->GetAnimInstance());
}
