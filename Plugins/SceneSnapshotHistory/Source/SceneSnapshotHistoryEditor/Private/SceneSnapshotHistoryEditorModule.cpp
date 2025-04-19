#include "SceneSnapshotHistoryEditorModule.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SSceneSnapshotHistoryWidget.h"

#define LOCTEXT_NAMESPACE "FSceneSnapshotHistoryEditorModule"

static const FName SceneSnapshotHistoryTabName("SceneSnapshotHistory");

void FSceneSnapshotHistoryEditorModule::StartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        SceneSnapshotHistoryTabName,
		                        FOnSpawnTab::CreateStatic(&FSceneSnapshotHistoryEditorModule::OnSpawnPluginTab)
	                        )
	                        .SetDisplayName(LOCTEXT("TabTitle", "Scene Snapshots"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> Extender = MakeShareable(new FExtender());
	Extender->AddMenuBarExtension(
		"Window",
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateRaw(this, &FSceneSnapshotHistoryEditorModule::AddMenuBarExtension)
	);

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(Extender);
}

void FSceneSnapshotHistoryEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SceneSnapshotHistoryTabName);
}

TSharedRef<SDockTab> FSceneSnapshotHistoryEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SSceneSnapshotHistoryWidget)
		];
}

void FSceneSnapshotHistoryEditorModule::AddMenuBarExtension(FMenuBarBuilder& Builder)
{
	Builder.AddPullDownMenu(
		LOCTEXT("MenuSceneSnapshots", "Snapshots"),
		LOCTEXT("MenuSnapshots_Tooltip", "Open Scene Snapshot History"),
		FNewMenuDelegate::CreateRaw(this, &FSceneSnapshotHistoryEditorModule::FillPulldownMenu),
		"Snapshots"
	);
}

void FSceneSnapshotHistoryEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SceneSnapshotHistoryTabName);
}

void FSceneSnapshotHistoryEditorModule::FillPulldownMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("OpenSnapshotHistory", "Open Snapshot History"),
		LOCTEXT("OpenSnapshotHistory_Tooltip", "Open the Scene Snapshot History tab"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FSceneSnapshotHistoryEditorModule::PluginButtonClicked))
	);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSceneSnapshotHistoryEditorModule, SceneSnapshotHistoryEditor)
