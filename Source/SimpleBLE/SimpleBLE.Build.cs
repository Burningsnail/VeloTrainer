using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;

public class SimpleBLE : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../../ThirdParty/" ) ); }
    }

    public SimpleBLE(ReadOnlyTargetRules Target) : base(Target)
    {
        // Settings go here
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            });
    }

    public bool LoadSimpleBLE(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if((Target.Platform == UnrealTargetPlatform.Win64) 
            ||
 (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "SimpleBLE", "Libraries");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "SimpleBLE." + PlatformString + ".lib")); 
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add( Path.Combine( ThirdPartyPath, "SimpleBLE", "Includes" ) );
        }

        PublicDefinitions.Add(string.Format( "WITH_SIMPLE_BLE_BINDING={0}", isLibrarySupported ? 1 : 0 ) );

        return isLibrarySupported;
    }

}