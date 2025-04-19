using UnrealBuildTool;

public class SceneSnapshotHistoryEditor : ModuleRules
{
	public SceneSnapshotHistoryEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"SlateCore",
				"InputCore" 
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"SceneSnapshotHistory"
			}
		);
	}
}