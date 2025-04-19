#include "SSceneSnapshotHistoryWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Views/SListView.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/CoreStyle.h"
#include "ImageUtils.h"
#include "Editor.h"
#include "Selection.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSceneSnapshotHistoryWidget::Construct(const FArguments& InArgs)
{
	// --- Setup dropdown ---
	SnapshotModeOptions.Add(MakeShared<FString>(TEXT("Scene Mode")));
	SnapshotModeOptions.Add(MakeShared<FString>(TEXT("Actor Mode")));
	SelectedSnapshotMode = SnapshotModeOptions[0];
	CurrentMode = ESnapshotCaptureMode::Scene;

	// --- Build initial list ---
	RebuildSnapshotList();

	ChildSlot
	[
		SNew(SVerticalBox)

		// Snapshot Name
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
				.OnTextChanged_Lambda([this](const FText& In) { SnapshotNameText = In; })
			]
		]

		// Mode Dropdown
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
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
				{
					return SNew(STextBlock).Text(FText::FromString(*Item));
				})
				.OnSelectionChanged_Lambda([this](TSharedPtr<FString> Sel, ESelectInfo::Type)
				{
					SelectedSnapshotMode = Sel;
					CurrentMode = (*Sel == TEXT("Actor Mode"))
						              ? ESnapshotCaptureMode::Selected
						              : ESnapshotCaptureMode::Scene;
				})
				.InitiallySelectedItem(SnapshotModeOptions[0])
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						return SelectedSnapshotMode.IsValid()
							       ? FText::FromString(*SelectedSnapshotMode)
							       : FText::FromString("Scene Mode");
					})
				]
			]
		]

		// Save / Refresh Buttons
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(2)
			[
				SNew(SButton)
				.Text(FText::FromString("Save Snapshot"))
				.OnClicked(this, &SSceneSnapshotHistoryWidget::OnSaveSnapshotClicked)
			]
			+ SHorizontalBox::Slot().AutoWidth().Padding(2)
			[
				SNew(SButton)
				.Text(FText::FromString("Refresh List"))
				.OnClicked(this, &SSceneSnapshotHistoryWidget::OnRefreshSnapshotListClicked)
			]
		]

		// Search Bar
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SEditableTextBox)
			.HintText(FText::FromString("Search snapshots..."))
			.OnTextChanged_Lambda([this](const FText& In)
			{
				SearchText = In;
				UpdateFilteredSnapshotList();
			})
		]

		// Snapshot List
		+ SVerticalBox::Slot().FillHeight(1.f).Padding(4)
		[
			SAssignNew(SnapshotListView, SListView<TSharedPtr<FSnapshotListEntry>>)
			.ItemHeight(72.f)
			.ListItemsSource(&SnapshotList)
			.OnGenerateRow(this, &SSceneSnapshotHistoryWidget::OnGenerateRow)
			.SelectionMode(ESelectionMode::None)
		]
	];
}

FReply SSceneSnapshotHistoryWidget::OnSaveSnapshotClicked()
{
	FName FinalName = SnapshotNameText.IsEmptyOrWhitespace()
		                  ? FName(TEXT("NewSnapshot"))
		                  : FName(*SnapshotNameText.ToString());

	if (GEditor)
	{
		UWorld* W = GEditor->GetEditorWorldContext().World();
		if (auto* Sub = W->GetSubsystem<USnapshotSubsystem>())
		{
			if (CurrentMode == ESnapshotCaptureMode::Scene)
			{
				Sub->SaveSnapshot(FinalName);
			}
			else
			{
				TArray<AActor*> Sel;
				GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(Sel);
				Sub->SaveSnapshot(FinalName, Sel);
			}

			RebuildSnapshotList();
			if (SnapshotListView.IsValid())
				SnapshotListView->RequestListRefresh();
		}
	}
	return FReply::Handled();
}

FReply SSceneSnapshotHistoryWidget::OnRefreshSnapshotListClicked()
{
	RebuildSnapshotList();
	if (SnapshotListView.IsValid())
		SnapshotListView->RequestListRefresh();
	return FReply::Handled();
}

void SSceneSnapshotHistoryWidget::RebuildSnapshotList()
{
	AllSnapshotsRaw.Empty();

	if (GEditor)
	{
		UWorld* W = GEditor->GetEditorWorldContext().World();
		if (auto* Sub = W->GetSubsystem<USnapshotSubsystem>())
		{
			for (auto const& Snap : Sub->GetSnapshots())
			{
				auto Entry = MakeShared<FSnapshotListEntry>();
				Entry->SnapshotName = Snap.SnapshotName;
				Entry->Timestamp = Snap.Timestamp;
				Entry->ThumbnailPath = Snap.ThumbnailPath;

				// **Load once here and store the brush**
				if (FPaths::FileExists(Entry->ThumbnailPath))
				{
					if (UTexture2D* Tex = FImageUtils::ImportFileAsTexture2D(Entry->ThumbnailPath))
					{
						Entry->ThumbnailBrush = MakeShared<FSlateImageBrush>(
							Tex,
							FVector2D(64.f, 64.f)
						);
					}
				}
				// fallback
				if (!Entry->ThumbnailBrush.IsValid())
				{
					Entry->ThumbnailBrush = MakeShared<FSlateBrush>(
						*FCoreStyle::Get().GetDefaultBrush()
					);
				}

				AllSnapshotsRaw.Add(Entry);
			}

			AllSnapshotsRaw.Sort([](auto& A, auto& B)
			{
				return A->Timestamp > B->Timestamp;
			});
		}
	}

	UpdateFilteredSnapshotList();
}

void SSceneSnapshotHistoryWidget::UpdateFilteredSnapshotList()
{
	const FString Filter = SearchText.ToString().ToLower();
	SnapshotList.Empty();

	for (auto& Entry : AllSnapshotsRaw)
	{
		if (Entry->SnapshotName.ToString().ToLower().Contains(Filter))
		{
			SnapshotList.Add(Entry);
		}
	}

	if (SnapshotListView.IsValid())
		SnapshotListView->RequestListRefresh();
}

TSharedRef<ITableRow> SSceneSnapshotHistoryWidget::OnGenerateRow(
	TSharedPtr<FSnapshotListEntry> InItem,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	// Use the pre‐built brush on the entry
	const TSharedPtr<FSlateBrush>& Brush = InItem->ThumbnailBrush;

	return SNew(STableRow<TSharedPtr<FSnapshotListEntry>>, OwnerTable)
		[
			SNew(SHorizontalBox)

			// Thumbnail
			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(SBox)
				.WidthOverride(64.f)
				.HeightOverride(64.f)
				[
					SNew(SImage)
					.Image(Brush.Get())
				]
			]

			// Name
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(8, 0)
			[
				SNew(STextBlock)
				.Text(FText::FromName(InItem->SnapshotName))
			]

			// Timestamp
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(8, 0)
			[
				SNew(STextBlock)
				.Text(FText::FromString(InItem->Timestamp.ToString()))
			]

			// Load
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

			// Delete
			+ SHorizontalBox::Slot().AutoWidth().Padding(4)
			[
				SNew(SButton)
				.Text(FText::FromString("Delete"))
				.OnClicked_Lambda([this, InItem]()
				{
					OnDeleteSnapshot(InItem->SnapshotName, InItem->Timestamp);
					RebuildSnapshotList();
					return FReply::Handled();
				})
			]
		];
}

void SSceneSnapshotHistoryWidget::OnRestoreSnapshot(FName SnapshotName, FDateTime Timestamp) const
{
	if (GEditor)
	{
		UWorld* W = GEditor->GetEditorWorldContext().World();
		if (auto* Sub = W->GetSubsystem<USnapshotSubsystem>())
		{
			Sub->RestoreSnapshot(SnapshotName, Timestamp);
		}
	}
}

void SSceneSnapshotHistoryWidget::OnDeleteSnapshot(FName SnapshotName, FDateTime Timestamp)
{
	if (GEditor)
	{
		UWorld* W = GEditor->GetEditorWorldContext().World();
		if (auto* Sub = W->GetSubsystem<USnapshotSubsystem>())
		{
			Sub->DeleteSnapshot(SnapshotName, Timestamp);
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
