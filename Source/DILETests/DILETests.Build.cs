// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;

public class DILETests : ModuleRules
{
    public DILETests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("DILETests");

        PublicDependencyModuleNames.AddRange(
        [
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
        ]);

        PrivateDependencyModuleNames.AddRange(
        [
            "DILE",
            "Slate",
            "SlateCore",
            "UE5Coro",
            "UMG",
            "UnrealDI",
            "UnrealEd",
            "UnrealMvvm",
        ]);
    }
}
