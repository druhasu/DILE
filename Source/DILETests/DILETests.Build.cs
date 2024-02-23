// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;

public class DILETests : ModuleRules
{
    public DILETests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("DILETests");

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "DILE",
                "Slate",
                "SlateCore",
                "UnrealDI",
                "UnrealEd",
                "UnrealMvvm",
            }
        );
    }
}
