#include "SnapshotSubsystem.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Editor.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Modules/ModuleManager.h"

void USnapshotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void USnapshotSubsystem::Deinitialize()
{
}

void USnapshotSubsystem::SaveSnapshot(FName SnapshotName)
{
	TArray<AActor*> AllActors;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AllActors.Add(*It);
	}
	return SaveSnapshot(SnapshotName, AllActors);
}

void USnapshotSubsystem::SaveSnapshot(FName SnapshotName, const TArray<AActor*>& ActorsToSave)
{
	// Build the snapshot data
	FSceneSnapshot Snap;
	Snap.SnapshotName = SnapshotName;
	Snap.Timestamp = FDateTime::Now();

	for (AActor* Actor : ActorsToSave)
	{
		if (!IsValid(Actor)) continue;
		FSceneSnapshotActorData Data;
		Data.ActorName = Actor->GetFName();
		Data.ActorTransform = Actor->GetActorTransform();
		Data.bVisible = !Actor->IsHidden();
		Snap.ActorStates.Add(MoveTemp(Data));
	}

	// Capture & record a thumbnail
	Snap.ThumbnailPath = SaveThumbnailForSnapshot(SnapshotName, Snap.Timestamp);

	// Store it
	SavedSnapshots.Add(MoveTemp(Snap));
}

void USnapshotSubsystem::SaveSnapshotAsync(FName SnapshotName)
{
	if (!GetWorld()) 
	{
		return;
	}

	AsyncTask(ENamedThreads::GameThread, [this, SnapshotName]()
	{
		SaveSnapshot(SnapshotName);
	});
}

void USnapshotSubsystem::CommitSnapshot(FName SnapshotName, TArray<FSceneSnapshotActorData>&& Collected)
{
	FSceneSnapshot Snap;
	Snap.SnapshotName = SnapshotName;
	Snap.Timestamp = FDateTime::Now();
	Snap.ActorStates = MoveTemp(Collected);

	SavedSnapshots.Add(MoveTemp(Snap));
}

void USnapshotSubsystem::RestoreSnapshot(FName SnapshotName, FDateTime Timestamp)
{
	const FSceneSnapshot* Found = FindSnapshot(SnapshotName, Timestamp);
	if (!Found) return;

	UWorld* World = GetWorld();
	for (const auto& Data : Found->ActorStates)
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->GetFName() == Data.ActorName)
			{
				Actor->SetActorTransform(Data.ActorTransform);
				Actor->SetActorHiddenInGame(!Data.bVisible);
				if (GEditor)
				{
					GEditor->NoteSelectionChange();
				}
				break;
			}
		}
	}
}

void USnapshotSubsystem::DeleteSnapshot(FName SnapshotName, FDateTime Timestamp)
{
	SavedSnapshots.RemoveAll([&](auto const& S)
	{
		return S.SnapshotName == SnapshotName && S.Timestamp == Timestamp;
	});
}

const TArray<FSceneSnapshot>& USnapshotSubsystem::GetSnapshots() const
{
	return SavedSnapshots;
}

const FSceneSnapshot* USnapshotSubsystem::FindSnapshot(FName SnapshotName, FDateTime Timestamp) const
{
	for (auto const& S : SavedSnapshots)
	{
		if (S.SnapshotName == SnapshotName && S.Timestamp == Timestamp)
		{
			return &S;
		}
	}
	return nullptr;
}

FString USnapshotSubsystem::SaveThumbnailForSnapshot(FName SnapshotName, FDateTime Timestamp)
{
	if (!GEditor) return TEXT("");

	// Ensure the folder exists
	FString Folder = FPaths::Combine(FPaths::ProjectIntermediateDir(), TEXT("SnapshotPreviews"));
	IFileManager::Get().MakeDirectory(*Folder, true);

	// Build a filename
	FString TimeStr = Timestamp.ToString().Replace(TEXT(":"), TEXT("-"));
	FString FileName = FString::Printf(TEXT("%s/%s_%s.png"),
	                                   *Folder, *SnapshotName.ToString(), *TimeStr);

	// Request a high‑res screenshot next frame
	FScreenshotRequest::RequestScreenshot(FileName, false, false);

	return FileName;
}
