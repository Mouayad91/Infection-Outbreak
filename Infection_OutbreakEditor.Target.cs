
using UnrealBuildTool;
using System.Collections.Generic;

public class Infection_OutbreakEditorTarget : TargetRules
{
	public Infection_OutbreakEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Infection_Outbreak");
	}
}
