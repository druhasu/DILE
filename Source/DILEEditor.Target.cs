// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DILEEditorTarget : TargetRules
{
    public DILEEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(
            new []
            {
                "DILE",
                "DILEEditor",
                "DILETests",
            }
        );
    }
}
