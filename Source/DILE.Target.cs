// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DILETarget : TargetRules
{
    public DILETarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(
            new []
            {
                "DILE",
            }
        );
    }
}
