﻿// GameServiceLocator.h

#pragma once

#include "CoreMinimal.h"
#include "../../Inventory/Components/InventoryComponent.h"
#include "../../Inventory/Services/InventoryService.h"
#include "../../Audio/Services/AudioService.h"
#include "GameServiceLocator.generated.h"

// Forward declarations of specific services
class UInventoryService;
class UAudioService;
class UInventoryComponent;

// UGameServiceLocator acts as a global registry for game-wide services,
// enabling other parts of the game to retrieve those services on demand
// This is the core of the Service Locator pattern implementation
UCLASS(Blueprintable)
class PROJECTSKYRUNNER_API UGameServiceLocator : public UObject
{
	GENERATED_BODY()

private:
	// A static map that holds all registered services
	// The key is a unique FName identifier for the service
	// The value is a UObject pointer to the actual service instance
	static TMap<FName, TWeakObjectPtr<UObject>> Services;

	// A weak pointer to the player's inventory component
	static TWeakObjectPtr<UInventoryComponent> PlayerInventoryComponent;

public:
	// Template function that retrieves a service by name
	// and attempts to cast it to the requested type
	// Returns nullptr if the service isn't found or cast fails
	template <typename T>
	static T* GetService(const FName ServiceName)
	{
		if (TWeakObjectPtr<UObject>* FoundService = Services.Find(ServiceName))
		{
			if (FoundService->IsValid())
			{
				return Cast<T>(FoundService->Get());
			}
		}
		return nullptr;
	}

	// Registers a service instance under the specified name
	// This allows it to be accessed globally from anywhere in the game
	UFUNCTION(BlueprintCallable, Category = "Services")
	static void RegisterService(const FName ServiceName, UObject* Service);

	// Unregisters a service by its name, removing it from the registry
	UFUNCTION(BlueprintCallable, Category = "Services")
	static void UnregisterService(const FName ServiceName);

	// Clears all registered services from the locator
	// Typically called during game shutdown
	UFUNCTION(BlueprintCallable, Category = "Services")
	static void ClearServices();

	// Returns the registered Inventory Service
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Services")
	static UInventoryService* GetInventoryService();

	// Returns the registered Audio Service
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Services")
	static UAudioService* GetAudioService();

	// Registers an inventory component
	UFUNCTION(BlueprintCallable, Category = "Services|Components")
	static void RegisterInventoryComponent(UInventoryComponent* InventoryComp);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Services|Components")
	// Retrieves the currently registered player inventory component
	static UInventoryComponent* GetPlayerInventoryComponent();
};
