// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DILETarget : TargetRules
{
    public DILETarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(
        [
            "DILE",
        ]);

        WindowsPlatform.CompilerVersion = "14.44.35207";
    }
}
