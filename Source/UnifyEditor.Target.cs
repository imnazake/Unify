// Copyright Nazake. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UnifyEditorTarget : TargetRules
{
	public UnifyEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Unify");
	}
}
