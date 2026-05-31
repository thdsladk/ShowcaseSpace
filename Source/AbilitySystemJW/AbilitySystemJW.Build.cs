// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AbilitySystemJW : ModuleRules
{
	public AbilitySystemJW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicIncludePaths.AddRange(new string[] { "AbilitySystemJW" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "ProjectJW",
            "GameFeatures",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags",
            "MotionWarping",
            "AIModule",
            "Niagara", 
            "NavigationSystem",
        });

        PrivateIncludePaths.AddRange(new string[]
       {
            "AbilitySystemJW",
            "AbilitySystemJW/Default",
            "AbilitySystemJW/Data",
            "AbilitySystemJW/Interface",
            "AbilitySystemJW/Header",
            "AbilitySystemJW/Prop",
            "AbilitySystemJW/Tag",
            "AbilitySystemJW/GA",
            "AbilitySystemJW/GA/Task",
            "AbilitySystemJW/GA/TargetActor",
            "AbilitySystemJW/GA/Effect",
            "AbilitySystemJW/GC",
            "AbilitySystemJW/Animation",
            "AbilitySystemJW/Character",
            "AbilitySystemJW/CharacterComponents",
            "AbilitySystemJW/Attribute",
            "AbilitySystemJW/UI",




       });


    }
}
