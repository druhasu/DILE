// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;

public class DILE : ModuleRules
{
    public DILE(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("DILE");

        PublicDependencyModuleNames.AddRange(
            new[]
            {
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
                "Slate",
                "SlateCore",
                "UnrealDI",
                "UnrealMvvm",
            }
        );
    }
}
