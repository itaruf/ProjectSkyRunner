#include "SnapshotBlueprintLibrary.h"
#include "SnapshotSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void USnapshotBlueprintLibrary::SaveSceneSnapshot(UObject* WorldContextObject, FName SnapshotName) {
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>()) {
		Subsystem->SaveSnapshot(SnapshotName);
	}
}

void USnapshotBlueprintLibrary::RestoreSceneSnapshot(UObject* WorldContextObject, FName SnapshotName) {
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (USnapshotSubsystem* Subsystem = World->GetSubsystem<USnapshotSubsystem>()) {
		Subsystem->RestoreSnapshot(SnapshotName);
	}
}
