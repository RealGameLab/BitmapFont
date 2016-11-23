using UnrealBuildTool;

public class BitmapFont : ModuleRules
{
	public BitmapFont(TargetInfo Target)
	{
		
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
                "AssetRegistry",
            }
            );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "UnrealEd",
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
