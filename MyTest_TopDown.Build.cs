// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyTest_TopDown : ModuleRules
{
	public MyTest_TopDown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		//PrivatePCHHeaderFile = "Private/PCH.h";


		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", 
			"NavigationSystem", "AIModule", "Niagara", "EnhancedInput","UMG",
			"GameplayTasks", "SlateCore","PhysicsCore","StructUtils","GameplayTags",
			"AbilitySystem",
        });


        PrivateIncludePaths.AddRange(new string[]
        {
			"MyTest_TopDown",
			"MyTest_TopDown/Public",
			"MyTest_TopDown/Private",
			"MyTest_TopDown/Default",
			"MyTest_TopDown/Character",
			"MyTest_TopDown/CharacterComponent",
			"MyTest_TopDown/Utill",
			"MyTest_TopDown/AI",
			"MyTest_TopDown/AI/Task",
			"MyTest_TopDown/AI/Service",
			"MyTest_TopDown/AI/Deco",
			"MyTest_TopDown/Monster",
			"MyTest_TopDown/Item",
			"MyTest_TopDown/UI",
			"MyTest_TopDown/Data",
			"MyTest_TopDown/Interface",
			"MyTest_TopDown/Skill",
			"MyTest_TopDown/Physics",
			"MyTest_TopDown/Header",
            "MyTest_TopDown/Game",
            "MyTest_TopDown/Animation",
			"MyTest_TopDown/Ability",
            "MyTest_TopDown/Ability/Task",
            "MyTest_TopDown/Projectile",


        });
	}
}
