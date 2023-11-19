// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class VeloTrainer3EditorTarget : TargetRules
{
	public VeloTrainer3EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "VeloTrainer3", "UEAdapterSimpleBLE" } );
	}
}
