// SkyRunnerGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SkyRunnerGameInstance.generated.h"

// This custom GameInstance class is responsible for setting up
// and tearing down global services used throughout the game
// It provides a centralized location to register services
// with the Service Locator during initialization
UCLASS()
class PROJECTSKYRUNNER_API USkyRunnerGameInstance: public UGameInstance
{
	GENERATED_BODY()

public:
	// Called once at the very start of the game, before any levels are loaded.
	// Marked as BlueprintNativeEvent to allow Blueprint overrides.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInstance")
	void Init() override;
	virtual void Init_Implementation();

	// Called when the game is shutting down.
	// Marked as BlueprintNativeEvent to allow Blueprint overrides.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInstance")
	void Shutdown() override;
	virtual void Shutdown_Implementation();
};