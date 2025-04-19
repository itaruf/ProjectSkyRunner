#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
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
};

UCLASS()
class SCENESNAPSHOTHISTORY_API USnapshotSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	UFUNCTION(CallInEditor, Category="Snapshots")
	void SaveSnapshot(FName SnapshotName);

	void SaveSnapshot(FName SnapshotName, const TArray<AActor*>& ActorsToSave);

	UFUNCTION(CallInEditor, Category="Snapshots")
	void RestoreSnapshot(FName SnapshotName, FDateTime Timestamp);

	UFUNCTION(CallInEditor, Category="Snapshots")
	void DeleteSnapshot(FName SnapshotName, FDateTime Timestamp);

	const TArray<FSceneSnapshot>& GetSnapshots() const;

private:
	TArray<FSceneSnapshot> SavedSnapshots;
	const FSceneSnapshot* FindSnapshot(FName SnapshotName, FDateTime Timestamp) const;
};
