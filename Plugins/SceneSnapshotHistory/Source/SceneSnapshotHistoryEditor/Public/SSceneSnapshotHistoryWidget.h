#pragma once
#include "Widgets/SCompoundWidget.h"
#include "SnapshotSubsystem.h"
#include "Widgets/Views/SListView.h"

struct FSnapshotListEntry
{
	FName SnapshotName;
	FDateTime Timestamp;
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
	TArray<TSharedPtr<FSnapshotListEntry>> SnapshotList;
	TSharedPtr<SListView<TSharedPtr<FSnapshotListEntry>>> SnapshotListView;
	FText SnapshotNameText;

	TArray<TSharedPtr<FString>> SnapshotModeOptions;
	TSharedPtr<FString> SelectedSnapshotMode;
	ESnapshotCaptureMode CurrentMode = ESnapshotCaptureMode::Scene;

	FReply OnSaveSnapshotClicked();
	FReply OnRefreshSnapshotListClicked();

	void RebuildSnapshotList();

	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FSnapshotListEntry> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void OnRestoreSnapshot(FName SnapshotName, FDateTime Timestamp) const;
	void OnDeleteSnapshot(FName SnapshotName, FDateTime Timestamp);
};
