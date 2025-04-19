#include "SceneSnapshotHistoryEditorModule.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SSceneSnapshotHistoryWidget.h"

#define LOCTEXT_NAMESPACE "FSceneSnapshotHistoryEditorModule"

static const FName SceneSnapshotHistoryTabName("SceneSnapshotHistory");

void FSceneSnapshotHistoryEditorModule::StartupModule()
{
	// 1) register our tab
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        SceneSnapshotHistoryTabName,
		                        FOnSpawnTab::CreateStatic(&FSceneSnapshotHistoryEditorModule::OnSpawnPluginTab)
	                        )
	                        .SetDisplayName(LOCTEXT("TabTitle", "Scene Snapshots"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	// 2) add menu‑bar extender
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LE = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		MenuExtender = MakeShared<FExtender>();
		MenuExtender->AddMenuBarExtension(
			"Window",
			EExtensionHook::After,
			nullptr,
			FMenuBarExtensionDelegate::CreateRaw(this, &FSceneSnapshotHistoryEditorModule::AddMenuBarExtension)
		);
		LE.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
}

void FSceneSnapshotHistoryEditorModule::ShutdownModule()
{
	// Unregister our tab
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SceneSnapshotHistoryTabName);

	// Remove our menu extender if LevelEditor is still around
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor") && MenuExtender.IsValid())
	{
		FLevelEditorModule& LE = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		LE.GetMenuExtensibilityManager()->RemoveExtender(MenuExtender);
		MenuExtender.Reset();
	}
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
