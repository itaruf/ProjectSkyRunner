#include "SSceneSnapshotHistoryWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Editor.h"
#include "Selection.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSceneSnapshotHistoryWidget::Construct(const FArguments& InArgs)
{
	RebuildSnapshotList();

	SnapshotModeOptions.Add(MakeShared<FString>(TEXT("Scene Mode")));
	SnapshotModeOptions.Add(MakeShared<FString>(TEXT("Actor Mode")));
	SelectedSnapshotMode = SnapshotModeOptions[0];
	CurrentMode = ESnapshotCaptureMode::Scene;

	ChildSlot
	[
		SNew(SVerticalBox)

		// Snapshot Name Input
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(2)
			[
				SNew(STextBlock).Text(FText::FromString("Snapshot Name:"))
			]
			+ SHorizontalBox::Slot().FillWidth(1.f).Padding(2)
			[
				SNew(SEditableTextBox)
				.Text_Lambda([this]() { return SnapshotNameText; })
				.OnTextChanged_Lambda([this](const FText& NewText) { SnapshotNameText = NewText; })
			]
		]

		// Snapshot Mode Dropdown
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(2)
			[
				SNew(STextBlock).Text(FText::FromString("Snapshot Mode:"))
			]
			+ SHorizontalBox::Slot().FillWidth(1.f).Padding(2)
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&SnapshotModeOptions)
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
				{
					return SNew(STextBlock).Text(FText::FromString(*InItem));
				})
				.OnSelectionChanged_Lambda([this](TSharedPtr<FString> NewSelection, ESelectInfo::Type)
				{
					SelectedSnapshotMode = NewSelection;
					if (*NewSelection == TEXT("Actor Mode"))
					{
						CurrentMode = ESnapshotCaptureMode::Selected;
					}
					else
					{
						CurrentMode = ESnapshotCaptureMode::Scene;
					}
				})
				.InitiallySelectedItem(SnapshotModeOptions[0])
				[
					SNew(STextBlock).Text_Lambda([this]()
					{
						return SelectedSnapshotMode.IsValid() ? FText::FromString(*SelectedSnapshotMode) : FText::FromString("Scene Mode");
					})
				]
			]
		]

		// Save Snapshot Button
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SButton)
			.Text(FText::FromString("Save Snapshot"))
			.OnClicked(this, &SSceneSnapshotHistoryWidget::OnSaveSnapshotClicked)
		]

		// Refresh List Button
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SButton)
			.Text(FText::FromString("Refresh List"))
			.OnClicked(this, &SSceneSnapshotHistoryWidget::OnRefreshSnapshotListClicked)
		]

		// Snapshot List
		+ SVerticalBox::Slot().FillHeight(1.f).Padding(4)
		[
			SAssignNew(SnapshotListView, SListView<TSharedPtr<FSnapshotListEntry>>)
			.ItemHeight(24)
			.ListItemsSource(&SnapshotList)
			.OnGenerateRow(this, &SSceneSnapshotHistoryWidget::OnGenerateRow)
			.SelectionMode(ESelectionMode::None)
		]
	];
}


FReply SSceneSnapshotHistoryWidget::OnSaveSnapshotClicked()
{
	FName FinalName = SnapshotNameText.IsEmptyOrWhitespace() ? FName(TEXT("NewSnapshot")) : FName(*SnapshotNameText.ToString());

	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>())
		{
			if (CurrentMode == ESnapshotCaptureMode::Scene)
			{
				Subsystem->SaveSnapshot(FinalName);
			}
			else if (CurrentMode == ESnapshotCaptureMode::Selected)
			{
				TArray<AActor*> SelectedActors;
				GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);
				Subsystem->SaveSnapshot(FinalName, SelectedActors);
			}

			RebuildSnapshotList();
			if (SnapshotListView.IsValid())
			{
				SnapshotListView->RequestListRefresh();
			}
		}
	}

	return FReply::Handled();
}

FReply SSceneSnapshotHistoryWidget::OnRefreshSnapshotListClicked()
{
	RebuildSnapshotList();
	if (SnapshotListView.IsValid())
	{
		SnapshotListView->RequestListRefresh();
	}
	return FReply::Handled();
}

void SSceneSnapshotHistoryWidget::RebuildSnapshotList()
{
	SnapshotList.Empty();

	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>())
		{
			const TArray<FSceneSnapshot>& Snapshots = Subsystem->GetSnapshots();
			for (const FSceneSnapshot& Snap : Snapshots)
			{
				TSharedPtr<FSnapshotListEntry> Entry = MakeShared<FSnapshotListEntry>();
				Entry->SnapshotName = Snap.SnapshotName;
				Entry->Timestamp = Snap.Timestamp;
				SnapshotList.Add(Entry);
			}

			SnapshotList.Sort([](const TSharedPtr<FSnapshotListEntry>& A, const TSharedPtr<FSnapshotListEntry>& B)
			{
				return A->Timestamp > B->Timestamp;
			});
		}
	}
}

TSharedRef<ITableRow> SSceneSnapshotHistoryWidget::OnGenerateRow(TSharedPtr<FSnapshotListEntry> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FSnapshotListEntry>>, OwnerTable)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(STextBlock).Text(FText::FromName(InItem->SnapshotName))
			]

			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(STextBlock).Text(FText::FromString(InItem->Timestamp.ToString()))
			]

			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(SButton)
				.Text(FText::FromString("Load"))
				.OnClicked_Lambda([this, InItem]()
				{
					OnRestoreSnapshot(InItem->SnapshotName, InItem->Timestamp);
					return FReply::Handled();
				})
			]

			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(SButton)
				.Text(FText::FromString("Delete"))
				.OnClicked_Lambda([this, InItem]()
				{
					OnDeleteSnapshot(InItem->SnapshotName, InItem->Timestamp);
					RebuildSnapshotList();
					SnapshotListView->RequestListRefresh();
					return FReply::Handled();
				})
			]
		];
}

void SSceneSnapshotHistoryWidget::OnRestoreSnapshot(FName SnapshotName, FDateTime Timestamp) const
{
	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>())
		{
			Subsystem->RestoreSnapshot(SnapshotName, Timestamp);
		}
	}
}

void SSceneSnapshotHistoryWidget::OnDeleteSnapshot(FName SnapshotName, FDateTime Timestamp)
{
	if (GEditor)
	{
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>())
		{
			Subsystem->DeleteSnapshot(SnapshotName, Timestamp);
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
