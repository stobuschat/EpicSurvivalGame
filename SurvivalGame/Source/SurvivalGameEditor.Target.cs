// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivalGameEditorTarget : TargetRules
{
	public SurvivalGameEditorTarget(TargetInfo Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.V5;
        Type = TargetType.Editor;
        ExtraModuleNames.AddRange(new string[] { "SurvivalGame" });
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
    }
}
