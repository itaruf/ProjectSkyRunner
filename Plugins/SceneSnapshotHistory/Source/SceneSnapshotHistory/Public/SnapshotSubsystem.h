#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Async/AsyncWork.h"
#include "SnapshotSubsystem.generated.h"


USTRUCT()
struct FSceneSnapshotActorData
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	bool bVisible;
};

USTRUCT()
struct FSceneSnapshot
{
	GENERATED_BODY()

	UPROPERTY()
	FName SnapshotName;

	UPROPERTY()
	FDateTime Timestamp;

	UPROPERTY()
	TArray<FSceneSnapshotActorData> ActorStates;

	/** Absolute path to the saved thumbnail PNG */
	UPROPERTY()
	FString ThumbnailPath;
};

UCLASS()
class SCENESNAPSHOTHISTORY_API USnapshotSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Required overrides
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	/** Save entire scene */
	UFUNCTION(CallInEditor, Category = "Snapshots")
	void SaveSnapshot(FName SnapshotName);

	/** Save only selected actors */
	void SaveSnapshot(FName SnapshotName, const TArray<AActor*>& ActorsToSave);

	/** Kicks off an async snapshot; UI will stay responsive. */
	UFUNCTION(CallInEditor, Category="Snapshots")
	void SaveSnapshotAsync(FName SnapshotName);

	/** (Internal) actually commit a snapshot on the game thread. */
	void CommitSnapshot(FName SnapshotName, TArray<FSceneSnapshotActorData>&& Collected);

	/** Restore by name + timestamp */
	UFUNCTION(CallInEditor, Category = "Snapshots")
	void RestoreSnapshot(FName SnapshotName, FDateTime Timestamp);

	/** Delete a single snapshot */
	UFUNCTION(CallInEditor, Category = "Snapshots")
	void DeleteSnapshot(FName SnapshotName, FDateTime Timestamp);

	/** All snapshots in memory */
	const TArray<FSceneSnapshot>& GetSnapshots() const;

private:
	/** In‑memory storage */
	TArray<FSceneSnapshot> SavedSnapshots;

	/** Helper to find one */
	const FSceneSnapshot* FindSnapshot(FName SnapshotName, FDateTime Timestamp) const;

	/** 
	 * Captures the current viewport into a PNG under Intermediate/SnapshotPreviews
	 * and returns the absolute filename.
	 */
	FString SaveThumbnailForSnapshot(FName SnapshotName, FDateTime Timestamp);
};
