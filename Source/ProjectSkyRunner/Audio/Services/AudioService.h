// AudioService.h

#pragma once

#include "CoreMinimal.h"
#include "../../Core/ServiceLocator/Interfaces/GameServiceInterface.h"
#include "AudioService.generated.h"

/* 
// The AudioService handles simple audio playback functionality
// It follows the IGameService interface, allowing it to be managed
// by the Service Locator and integrated cleanly across systems
*/
UCLASS(Blueprintable)
class PROJECTSKYRUNNER_API UAudioService : public UObject, public IGameServiceInterface
{
	GENERATED_BODY()

public:
	// Constructor
	UAudioService();

	// Begin IGameService interface

	// // Called when the service is initialized (e.g., from GameInstance::Init)
	// // Use this to set up any internal state or audio-related resources
	// virtual void InitializeService() override;
	//
	// // Called when the service is shutting down
	// // This is your opportunity to clean up any remaining references or data
	// virtual void ShutdownService() override;

	// Override the blueprint native event implementations:
	virtual void InitializeService_Implementation() override;
	virtual void ShutdownService_Implementation() override;

	// End IGameService interface

	// Plays the specified sound at the provided world location
	// This method can be used by any system that needs to trigger audio feedback,
	// such as UI prompts, environmental cues, or gameplay events
	UFUNCTION(BlueprintCallable, Category="Audio")
	void PlaySoundAtLocation(USoundBase* Sound, FVector Location);
};