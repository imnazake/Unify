// Copyright Nazake. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UnifyTarget : TargetRules
{
	public UnifyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Unify");
	}
}
