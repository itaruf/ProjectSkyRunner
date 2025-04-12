// SkyRunnerGameInstance.cpp

#include "SkyRunnerGameInstance.h"
#include "../Audio/Services/AudioService.h"
#include "../Core/ServiceLocator/GameServiceLocator.h"
#include "../Inventory/Services/InventoryService.h"

void USkyRunnerGameInstance::Init_Implementation()
{
	Super::Init();

	// Create an instance of the Inventory Service and register it
	// This makes the inventory system globally accessible throughout the game
	UInventoryService* InventoryService = NewObject<UInventoryService>(this, UInventoryService::StaticClass());
	if (InventoryService)
	{
		IGameServiceInterface::Execute_InitializeService(InventoryService); // Prepare the service (e.g., clear internal state)
		UGameServiceLocator::RegisterService("Inventory", InventoryService);
	}

	// Create an instance of the Audio Service and register it
	// Enables any Actor or system to request sound playback globally
	UAudioService* AudioService = NewObject<UAudioService>(this, UAudioService::StaticClass());
	if (AudioService)
	{
		IGameServiceInterface::Execute_InitializeService(AudioService); // Prepare any audio-related setup if needed
		UGameServiceLocator::RegisterService("Audio", AudioService);
	}
}

void USkyRunnerGameInstance::Shutdown_Implementation()
{
	// Gracefully shut down all services before clearing the registry
	// This allows each service to clean up any resources it holds

	if (UInventoryService* InventoryService = UGameServiceLocator::GetService<UInventoryService>("Inventory"))
	{
		IGameServiceInterface::Execute_ShutdownService(InventoryService);
	}

	if (UAudioService* AudioService = UGameServiceLocator::GetService<UAudioService>("Audio"))
	{
		IGameServiceInterface::Execute_ShutdownService(AudioService);
	}

	// Remove all service registrations to ensure a clean shutdown
	UGameServiceLocator::ClearServices();

	Super::Shutdown();
}
