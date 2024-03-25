

using UnrealBuildTool;

public class AppImageModule : ModuleRules
{
	public AppImageModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"HTTP",
			"Json",
			"AppCommonModule"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Engine",
			"CoreUObject"
		});

		if (Target.bBuildEditor) PrivateDependencyModuleNames.Add("UnrealEd");
	}
}