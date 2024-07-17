// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class FF_DB_NANODBC : ModuleRules
{
	public FF_DB_NANODBC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;

        if (UnrealTargetPlatform.Win64 == Target.Platform)
        {
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "nanodbc", "Win64", "include"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty", "nanodbc", "Win64", "lib", "nanodbc.lib"));
        }

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
				"Json",
                "JsonUtilities",
                "JsonBlueprintUtilities",
			}
			);

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
			);
	}
}
