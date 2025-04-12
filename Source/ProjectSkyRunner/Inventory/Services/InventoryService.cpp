// InventoryService.cpp

#include "InventoryService.h"

// Constructor
UInventoryService::UInventoryService()
{
	// Any setup logic could go here, but it's typically empty...
}

UInventoryService* UInventoryService::CreateInventoryService(UObject* WorldContext)
{
	return NewObject<UInventoryService>(WorldContext);
}

// Called during service initialization (e.g., from GameInstance::Init)
void UInventoryService::InitializeService_Implementation()
{
	Inventory.Empty(); // Start with a clean inventory
	UE_LOG(LogTemp, Log, TEXT("Inventory Service Initialized"));
}

// Called when the service is shutting down (e.g., GameInstance::Shutdown)
void UInventoryService::ShutdownService_Implementation()
{
	Inventory.Empty(); // Clear stored items to avoid lingering data
	UE_LOG(LogTemp, Log, TEXT("Inventory Service Shutdown"));
}

// Adds a specified quantity of the item to the inventory
// If the item is already tracked, it increments the count
void UInventoryService::AddItem(const FName ItemID, const int32 Quantity)
{
	if (Inventory.Contains(ItemID))
	{
		Inventory[ItemID] += Quantity;
	}
	else
	{
		Inventory.Add(ItemID, Quantity);
	}

	UE_LOG(LogTemp, Log, TEXT("Added %d of %s"), Quantity, *ItemID.ToString());
}

// Removes a given quantity of the item from the inventory
// If the item count reaches zero or less, it is removed entirely
bool UInventoryService::RemoveItem(const FName ItemID, const int32 Quantity)
{
	if (Inventory.Contains(ItemID) && Inventory[ItemID] >= Quantity)
	{
		Inventory[ItemID] -= Quantity;

		if (Inventory[ItemID] <= 0)
		{
			Inventory.Remove(ItemID);
		}

		UE_LOG(LogTemp, Log, TEXT("Removed %d of %s"), Quantity, *ItemID.ToString());
		return true;
	}

	// Removal failed: item doesn't exist or not enough quantity
	return false;
}

// Returns the current quantity of a given item
// If the item doesn't exist in the inventory, returns 0
int32 UInventoryService::GetItemCount(const FName ItemID) const
{
	const int32* Count = Inventory.Find(ItemID);
	return Count ? *Count : 0;
}