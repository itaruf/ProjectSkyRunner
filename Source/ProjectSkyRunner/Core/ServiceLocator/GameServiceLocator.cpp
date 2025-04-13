// GameServiceLocator.cpp

#include "GameServiceLocator.h"

// Define the static container
TMap<FName, TWeakObjectPtr<UObject>> UGameServiceLocator::Services;
TWeakObjectPtr<UInventoryComponent> UGameServiceLocator::PlayerInventoryComponent = nullptr;

void UGameServiceLocator::RegisterService(const FName ServiceName, UObject* Service)
{
	if (IsValid(Service))
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
	if (TWeakObjectPtr<UObject>* FoundService = Services.Find("Inventory"))
	{
		if (FoundService->IsValid())
		{
			return Cast<UInventoryService>(FoundService->Get());
		}
	}
	return nullptr;
}

// Returns the registered Audio Service
UAudioService* UGameServiceLocator::GetAudioService()
{
	if (TWeakObjectPtr<UObject>* FoundService = Services.Find("Audio"))
	{
		if (FoundService->IsValid())
		{
			return Cast<UAudioService>(FoundService->Get());
		}
	}
	return nullptr;
}
