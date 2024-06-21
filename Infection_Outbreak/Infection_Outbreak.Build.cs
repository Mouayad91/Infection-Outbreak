// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Infection_Outbreak : ModuleRules
{
	public Infection_Outbreak(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
