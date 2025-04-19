#include "SnapshotSubsystem.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Editor.h"

void USnapshotSubsystem::Initialize(FSubsystemCollectionBase& Collection) {}
void USnapshotSubsystem::Deinitialize() {}

void USnapshotSubsystem::SaveSnapshot(FName SnapshotName)
{
    // Check if snapshot with this name already exists
    for (FSceneSnapshot& Existing : SavedSnapshots)
    {
        if (Existing.SnapshotName == SnapshotName)
        {
            Existing.ActorStates.Empty();
            Existing.Timestamp = FDateTime::Now();

            UWorld* World = GetWorld();
            for (TActorIterator<AActor> It(World); It; ++It)
            {
                AActor* Actor = *It;
                FSceneSnapshotActorData Data;
                Data.ActorName = Actor->GetFName();
                Data.ActorTransform = Actor->GetActorTransform();
                Data.bVisible = !Actor->IsHidden();
                Existing.ActorStates.Add(Data);
            }
            return;
        }
    }

    // No existing snapshot found, create new one
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

void USnapshotSubsystem::RestoreSnapshot(FName SnapshotName) {
    const FSceneSnapshot* Snap = FindSnapshot(SnapshotName);
    if (!Snap) return;

    UWorld* World = GetWorld();
    for (const FSceneSnapshotActorData& Data : Snap->ActorStates) {
        for (TActorIterator<AActor> It(World); It; ++It) {
            if (It->GetFName() == Data.ActorName) {
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

const FSceneSnapshot* USnapshotSubsystem::FindSnapshot(FName SnapshotName) const {
    for (const FSceneSnapshot& S : SavedSnapshots) {
        if (S.SnapshotName == SnapshotName) return &S;
    }
    return nullptr;
}

const TArray<FSceneSnapshot>& USnapshotSubsystem::GetSnapshots() const {
    return SavedSnapshots;
}
