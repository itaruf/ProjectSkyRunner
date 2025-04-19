#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SnapshotBlueprintLibrary.generated.h"

UCLASS()
class SCENESNAPSHOTHISTORY_API USnapshotBlueprintLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"), Category="Snapshots")
	static void SaveSceneSnapshot(UObject* WorldContextObject, FName SnapshotName);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"), Category="Snapshots")
	static void RestoreSceneSnapshot(UObject* WorldContextObject, FName SnapshotName);
};
