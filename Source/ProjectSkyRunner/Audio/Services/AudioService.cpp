// AudioService.cpp

#include "AudioService.h"
#include "Kismet/GameplayStatics.h"

// Constructor
UAudioService::UAudioService()
{
	// Any required setup can go here
}

// Called when the service is initialized (e.g., from GameInstance::Init)
void UAudioService::InitializeService_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Audio Service Initialized"));
}

// Called when the service is shutting down
void UAudioService::ShutdownService_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Audio Service Shutdown"));
}

// Plays the specified sound at the provided world location
void UAudioService::PlaySoundAtLocation(USoundBase* Sound, FVector Location)
{
	if (Sound)
	{
		// Use the engine's gameplay statics utility to play the sound at the specified location
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
		UE_LOG(LogTemp, Log, TEXT("Played sound at location: %s"), *Location.ToString());
	}
}