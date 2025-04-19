#include "SnapshotSubsystem.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Editor.h"

void USnapshotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void USnapshotSubsystem::Deinitialize()
{
}

void USnapshotSubsystem::SaveSnapshot(FName SnapshotName)
{
	FSceneSnapshot Snap;
	Snap.SnapshotName = SnapshotName;
	Snap.Timestamp = FDateTime::Now();

	UWorld* World = GetWorld();
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		FSceneSnapshotActorData Data;
		Data.ActorName = Actor->GetFName();
		Data.ActorTransform = Actor->GetActorTransform();
		Data.bVisible = !Actor->IsHidden();
		Snap.ActorStates.Add(Data);
	}

	SavedSnapshots.Add(Snap);
}

void USnapshotSubsystem::RestoreSnapshot(FName SnapshotName, FDateTime Timestamp)
{
	const FSceneSnapshot* Snap = FindSnapshot(SnapshotName, Timestamp);
	if (!Snap) return;

	UWorld* World = GetWorld();
	for (const FSceneSnapshotActorData& Data : Snap->ActorStates)
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (It->GetFName() == Data.ActorName)
			{
				AActor* Actor = *It;
				Actor->SetActorTransform(Data.ActorTransform);
				Actor->SetActorHiddenInGame(!Data.bVisible);

				if (GEditor && Actor->IsSelectedInEditor())
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
	for (int32 i = 0; i < SavedSnapshots.Num(); ++i)
	{
		const FSceneSnapshot& S = SavedSnapshots[i];
		if (S.SnapshotName == SnapshotName && S.Timestamp == Timestamp)
		{
			SavedSnapshots.RemoveAt(i);
			break;
		}
	}
}

const FSceneSnapshot* USnapshotSubsystem::FindSnapshot(FName SnapshotName, FDateTime Timestamp) const
{
	for (const FSceneSnapshot& S : SavedSnapshots)
	{
		if (S.SnapshotName == SnapshotName && S.Timestamp == Timestamp)
		{
			return &S;
		}
	}
	return nullptr;
}

const TArray<FSceneSnapshot>& USnapshotSubsystem::GetSnapshots() const
{
	return SavedSnapshots;
}
