// InventoryService.h

#pragma once

#include "CoreMinimal.h"
#include "../../Core/ServiceLocator/Interfaces/GameServiceInterface.h"
#include "InventoryService.generated.h"

/*
// The InventoryService provides an interface for managing item throughout 
// the game. It adheres to the IGameService interface, making it compatible with 
// the Service Locator pattern.
*/
UCLASS(Blueprintable)
class PROJECTSKYRUNNER_API UInventoryService : public UObject, public IGameServiceInterface
{
	GENERATED_BODY()

public:
	// Constructor
	UInventoryService();

	// Static factory method for Blueprint instantiation
	UFUNCTION(BlueprintCallable, Category = "Services")
	static UInventoryService* CreateInventoryService(UObject* WorldContext);

	/* IGameService methods */

	// Override the blueprint native event implementations:
	// Called at service initialization (e.g., during GameInstance::Init)
	virtual void InitializeService_Implementation() override;
	// Called during shutdown or when the service is unregistered
	virtual void ShutdownService_Implementation() override;

	/* End IGameService methods */

	// Adds a specified quantity of the given item to the inventory
	// If the item does not exist, it is added to the map
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItem(const FName ItemID, const int32 Quantity = 1);

	// Removes a specified quantity of the given item from the inventory
	// Returns false if the item does not exist or not enough quantity is available
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(const FName ItemID, const int32 Quantity = 1);

	// Returns the current quantity of the given item in the inventory
	// If the item doesn't exist, returns 0
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetItemCount(const FName ItemID) const;
};
