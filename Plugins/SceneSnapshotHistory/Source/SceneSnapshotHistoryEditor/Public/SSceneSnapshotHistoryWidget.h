#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"

struct FSnapshotListEntry
{
	FName SnapshotName;
	FDateTime Timestamp;
	FString ThumbnailPath;

	// Keep the brush (and thus the UTexture2D) alive for this row:
	TSharedPtr<FSlateBrush> ThumbnailBrush;
};

enum class ESnapshotCaptureMode : uint8
{
	Scene,
	Selected
};

class SSceneSnapshotHistoryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSceneSnapshotHistoryWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// Raw + filtered lists
	TArray<TSharedPtr<FSnapshotListEntry>> AllSnapshotsRaw;
	TArray<TSharedPtr<FSnapshotListEntry>> SnapshotList;

	TSharedPtr<SListView<TSharedPtr<FSnapshotListEntry>>> SnapshotListView;

	// UI state
	FText SnapshotNameText;
	FText SearchText;
	TArray<TSharedPtr<FString>> SnapshotModeOptions;
	TSharedPtr<FString> SelectedSnapshotMode;
	ESnapshotCaptureMode CurrentMode = ESnapshotCaptureMode::Scene;

	// Callbacks
	FReply OnSaveSnapshotClicked();
	FReply OnRefreshSnapshotListClicked();
	void RebuildSnapshotList();
	void UpdateFilteredSnapshotList();

	TSharedRef<ITableRow> OnGenerateRow(
		TSharedPtr<FSnapshotListEntry> InItem,
		const TSharedRef<STableViewBase>& OwnerTable);

	void OnRestoreSnapshot(FName SnapshotName, FDateTime Timestamp) const;
	void OnDeleteSnapshot(FName SnapshotName, FDateTime Timestamp);
};
