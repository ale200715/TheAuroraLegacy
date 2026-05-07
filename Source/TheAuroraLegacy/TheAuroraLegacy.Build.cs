// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
public class TheAuroraLegacy : ModuleRules
{
    public TheAuroraLegacy(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // Agregar rutas de include
        PublicIncludePaths.AddRange(new string[] {
            "TheAuroraLegacy/Enemies",
            "TheAuroraLegacy/Player"
        });
    }
}
