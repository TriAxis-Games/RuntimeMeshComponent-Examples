// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RuntimeMeshSandboxTarget : TargetRules
{
	public RuntimeMeshSandboxTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("RuntimeMeshSandbox");
	}
}
