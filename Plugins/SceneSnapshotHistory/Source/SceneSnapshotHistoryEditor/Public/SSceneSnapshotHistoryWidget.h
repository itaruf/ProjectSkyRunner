#pragma once
#include "Widgets/SCompoundWidget.h"
#include "SnapshotSubsystem.h"

class SSceneSnapshotHistoryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSceneSnapshotHistoryWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnSaveSnapshotClicked() const;
	FReply OnRestoreSnapshotClicked_Internal() const;
	void OnRestoreSnapshot(FName SnapshotName, FDateTime Timestamp) const;
};
