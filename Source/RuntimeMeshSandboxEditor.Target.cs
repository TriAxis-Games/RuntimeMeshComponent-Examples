// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RuntimeMeshSandboxEditorTarget : TargetRules
{
	public RuntimeMeshSandboxEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("RuntimeMeshSandbox");
	}
}
