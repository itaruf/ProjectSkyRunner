#include "SSceneSnapshotHistoryWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSceneSnapshotHistoryWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)

		// Save Snapshot Button
		+ SVerticalBox::Slot().AutoHeight().Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString("Save Snapshot"))
			.OnClicked(this, &SSceneSnapshotHistoryWidget::OnSaveSnapshotClicked)
		]

		// Restore Snapshot Button
		+ SVerticalBox::Slot().AutoHeight().Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString("Restore Snapshot"))
			.OnClicked(this, &SSceneSnapshotHistoryWidget::OnRestoreSnapshotClicked_Internal)
		]
	];
}

FReply SSceneSnapshotHistoryWidget::OnSaveSnapshotClicked() const
{
	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>())
		{
			Subsystem->SaveSnapshot(FName(TEXT("NewSnapshot")));
		}
	}
	return FReply::Handled();
}

FReply SSceneSnapshotHistoryWidget::OnRestoreSnapshotClicked_Internal() const
{
	OnRestoreSnapshot(FName(TEXT("NewSnapshot")));
	return FReply::Handled();
}

void SSceneSnapshotHistoryWidget::OnRestoreSnapshot(FName SnapshotName) const
{
	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>())
		{
			Subsystem->RestoreSnapshot(SnapshotName);
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
