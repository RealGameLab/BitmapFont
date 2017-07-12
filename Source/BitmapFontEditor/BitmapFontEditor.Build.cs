using UnrealBuildTool;

public class BitmapFontEditor : ModuleRules
{
	public BitmapFontEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivatePCHHeaderFile = "Private/BMFontEditorPrivatePCH.h";
        PublicIncludePaths.AddRange(
			new string[] {
				"BitmapFontEditor/Public",
			}
			);

        PrivateIncludePaths.AddRange(
			new string[] {
                "BitmapFontEditor/Private",
            }
			);

        PublicIncludePathModuleNames.AddRange(
            new string[] {
                "AssetRegistry",
                "UnrealEd",
                "BitmapFont"
            }
            );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				
			}
			);
	}
}
