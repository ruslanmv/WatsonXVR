



using UnrealBuildTool;

public class WatsonxVREditorModule : ModuleRules
{
	public WatsonxVREditorModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Engine",
			"CoreUObject",
			"InputCore",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"ToolMenus",
			"EditorStyle",
			"WorkspaceMenuStructure",
			"Projects",
			"AssetRegistry",
			"WatsonxVRCommonModule",
			"WatsonxVRChatModule",
			"WatsonxVRImageModule",
			"Json"
		});
	}
}