



using UnrealBuildTool;

public class WatsonxVRCommonModule : ModuleRules
{
	public WatsonxVRCommonModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"HTTP",
			"Json"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Engine",
			"CoreUObject",
			"DeveloperSettings"
		});

		if (Target.bBuildEditor) PrivateDependencyModuleNames.Add("UnrealEd");
	}
}