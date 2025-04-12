// GameServiceLocator.cpp

#include "GameServiceLocator.h"

// Define the static container
TMap<FName, UObject*> UGameServiceLocator::Services;
TWeakObjectPtr<UInventoryComponent> UGameServiceLocator::PlayerInventoryComponent = nullptr;

void UGameServiceLocator::RegisterService(const FName ServiceName, UObject* Service)
{
	if (Service)
	{
		Services.Add(ServiceName, Service);
	}
}

void UGameServiceLocator::UnregisterService(const FName ServiceName)
{
	Services.Remove(ServiceName);
}

void UGameServiceLocator::ClearServices()
{
	Services.Empty();
}

void UGameServiceLocator::RegisterInventoryComponent(UInventoryComponent* InventoryComp)
{
	if (InventoryComp)
	{
		PlayerInventoryComponent = InventoryComp;
		UE_LOG(LogTemp, Log, TEXT("Player Inventory Component registered."));
	}
}

UInventoryComponent* UGameServiceLocator::GetPlayerInventoryComponent()
{
	return PlayerInventoryComponent.Get();
}

// Returns the registered Inventory Service
UInventoryService* UGameServiceLocator::GetInventoryService()
{
	return Cast<UInventoryService>(Services.FindRef("Inventory"));
}

// Returns the registered Audio Service
UAudioService* UGameServiceLocator::GetAudioService()
{
	return Cast<UAudioService>(Services.FindRef("Audio"));
}
