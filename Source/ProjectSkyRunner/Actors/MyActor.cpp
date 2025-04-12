// MyActor.cpp

#include "MyActor.h"
#include "../Audio/Services/AudioService.h"
#include "../Core/ServiceLocator/GameServiceLocator.h"
#include "../Inventory/Services/InventoryService.h"
#include "Sound/SoundBase.h"

void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// Attempt to retrieve the Inventory Service from the Service Locator
	// This allows us to interact with the inventory system without having
	// to maintain a direct reference to it
	if (UInventoryService* InventoryService = UGameServiceLocator::GetService<UInventoryService>("Inventory"))
	{
		// Add one HealthPotion to the inventory
		InventoryService->AddItem("HealthPotion", 1);

		// Retrieve and log the current count of HealthPotions in the inventory
		int32 Count = InventoryService->GetItemCount("HealthPotion");
		UE_LOG(LogTemp, Log, TEXT("HealthPotion Count: %d"), Count);
	}

	// Attempt to retrieve the Audio Service
	// This allows us to play a sound without depending on the audio system directly
	if (UAudioService* AudioService = UGameServiceLocator::GetService<UAudioService>("Audio"))
	{
		// If a sound has been assigned in the editor or Blueprint, play it at the Actor's location
		if (MySound)
		{
			AudioService->PlaySoundAtLocation(MySound, GetActorLocation());
		}
	}
}