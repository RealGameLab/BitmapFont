using UnrealBuildTool;

public class BitmapFont : ModuleRules
{
	public BitmapFont(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivatePCHHeaderFile = "Private/BMFontPrivatePCH.h";
        PublicIncludePaths.AddRange(
			new string[] {
				"BitmapFont/Public",
			}
			);

        

        PrivateIncludePaths.AddRange(
			new string[] {
				"BitmapFont/Private",
            }
			);

        PublicIncludePathModuleNames.AddRange(
            new string[] {
               // "UnrealEd",
                "SlateCore"
            }
            );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
               // "UnrealEd",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "ImageWrapper",
                "InputCore",
                "UMG",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
