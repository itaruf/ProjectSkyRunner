#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSceneSnapshotHistoryEditorModule : public IModuleInterface
{
public:
	void StartupModule() override;
	void ShutdownModule() override;

private:
	TSharedPtr<FExtender> MenuExtender;

	static TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
	void AddMenuBarExtension(FMenuBarBuilder& Builder);
	void PluginButtonClicked();
	void FillPulldownMenu(FMenuBuilder& MenuBuilder);
};
