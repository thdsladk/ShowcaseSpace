// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectJW : ModuleRules
{
	public ProjectJW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ProjectJW" });

        PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", 
			"EnhancedInput", "UMG", "Niagara", "NavigationSystem", "AIModule", "Slate", "SlateCore",
            "AnimGraphRuntime","MotionWarping",
        });


        PrivateIncludePaths.AddRange(new string[]
       {
            "ProjectJW",
            "ProjectJW/Default",
            "ProjectJW/Character",
            "ProjectJW/Components",
            //"ProjectJW/AI",
            //"ProjectJW/AI/Task",
            //"ProjectJW/AI/Service",
            //"ProjectJW/AI/Deco",
            //"ProjectJW/Item",
            "ProjectJW/UI",
            "ProjectJW/Manager",
            "ProjectJW/Data",
            "ProjectJW/Interface",
            "ProjectJW/Header",
            "ProjectJW/Game",
            "ProjectJW/Animation",
            "ProjectJW/Prop",
            //"ProjectJW/Projectile",
            "ProjectJW/Physics",


       });


    }
}
