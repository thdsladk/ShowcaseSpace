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
            "AbilitySystemJW/Animation",
            "AbilitySystemJW/Character",
            "AbilitySystemJW/Components",
            "AbilitySystemJW/Attribute",
            "AbilitySystemJW/UI",
            "AbilitySystemJW/Notify",
            "AbilitySystemJW/Items",
            
            "AbilitySystemJW/AbilitySystem",
            "AbilitySystemJW/AbilitySystem/GA",
            "AbilitySystemJW/AbilitySystem/GE",
            "AbilitySystemJW/AbilitySystem/GC",
            "AbilitySystemJW/AbilitySystem/AbilityTask",
            "AbilitySystemJW/AbilitySystem/TargetActor",

            "AbilitySystemJW/AI",


       });


    }
}
