



using UnrealBuildTool;

public class AppEditorModule : ModuleRules
{
	public AppEditorModule(ReadOnlyTargetRules Target) : base(Target)
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
			"AppCommonModule",
			"AppChatModule",
			"AppImageModule",
			"Json"
		});
	}
}