using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;
using System;

public class UEAdapterSimpleBLE : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../../ThirdParty/" ) ); }
    }

    public UEAdapterSimpleBLE(ReadOnlyTargetRules Target) : base(Target)
    {
        // Settings go here
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject", 
                "Engine", 
                "InputCore"
			}
		);
        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject"
			}
		);
        //PublicDefinitions.Add("WITH_UEADAPTERSIMPLEBLE=1");

        string LibrariesPath = Path.Combine(ThirdPartyPath, "UEAdapterSimpleBLE", "lib");
        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "simpleble.lib"));
        // PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "simpleble.dll"));
        PublicIncludePaths.Add( Path.Combine( ThirdPartyPath, "UEAdapterSimpleBLE", "include" ) );
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
            string LibrariesPath = Path.Combine(ThirdPartyPath, "SimpleBLE", "lib");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            //PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "SimpleBLE." + PlatformString + ".lib")); 
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "simpleble.lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add( Path.Combine( ThirdPartyPath, "SimpleBLE", "include", "simpleble" ) );
        }

        PublicDefinitions.Add(string.Format( "WITH_SIMPLE_BLE_BINDING={0}", isLibrarySupported ? 1 : 0 ) );

        return isLibrarySupported;
    }

}