

using UnrealBuildTool;

public class WatsonxVRImageModule : ModuleRules
{
	public WatsonxVRImageModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"HTTP",
			"Json",
			"WatsonxVRCommonModule"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Engine",
			"CoreUObject"
		});

		if (Target.bBuildEditor) PrivateDependencyModuleNames.Add("UnrealEd");
	}
}