// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DILEEditorTarget : TargetRules
{
    public DILEEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(
        [
            "DILE",
            "DILEEditor",
            "DILETests",
        ]);

        WindowsPlatform.CompilerVersion = "14.44.35207";
    }
}
