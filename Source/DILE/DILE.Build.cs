// Copyright Andrei Sudarikov. All Rights Reserved.

using UnrealBuildTool;

public class DILE : ModuleRules
{
    public DILE(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("DILE");

        PublicDependencyModuleNames.AddRange(
        [
            "AIModule",
            "Core",
            "CoreUObject",
            "DeveloperSettings",
            "DruInterfaceEvents",
            "Engine",
            "EnhancedInput",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "InputCore",
            "Mover",
            "NetworkPrediction",
            "UE5Coro",
            "UMG",
            "UnrealDI",
            "UnrealMvvm",
        ]);

        PrivateDependencyModuleNames.AddRange(
        [
            "NetCore",
            "Slate",
            "SlateCore",
        ]);
    }
}
