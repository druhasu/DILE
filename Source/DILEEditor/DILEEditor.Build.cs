// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;

public class DILEEditor : ModuleRules
{
    public DILEEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("DILEEditor");

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "AsyncMixin",
                "Core",
                "CoreUObject",
                "DeveloperSettings",
                "Engine",
                "InputCore",
                "UMG",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "BlueprintGraph",
                "DILE",
                "GraphEditor",
                "PropertyEditor",
                "Slate",
                "SlateCore",
                "UnrealDI",
                "UnrealEd",
                "UnrealMvvm",
            }
        );
    }
}
