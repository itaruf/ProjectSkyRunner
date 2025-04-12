// InventoryComponent.cpp

#pragma once

#include "InventoryComponent.h"
#include "../../Core/ServiceLocator/GameServiceLocator.h"
#include "GameFramework/Actor.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Self-register with a central system (for example, the service locator)
	// This avoids the InventoryService having to know directly about the owner
	UGameServiceLocator::RegisterInventoryComponent(this);
}

void UInventoryComponent::AddItem(const FName ItemID, const int32 Quantity)
{
	if (Items.Contains(ItemID))
	{
		Items[ItemID] += Quantity;
	}
	else
	{
		Items.Add(ItemID, Quantity);
	}
	UE_LOG(LogTemp, Log, TEXT("Added %d of %s"), Quantity, *ItemID.ToString());
}

bool UInventoryComponent::RemoveItem(const FName ItemID, const int32 Quantity)
{
	if (Items.Contains(ItemID) && Items[ItemID] >= Quantity)
	{
		Items[ItemID] -= Quantity;
		if (Items[ItemID] <= 0)
		{
			Items.Remove(ItemID);
		}
		UE_LOG(LogTemp, Log, TEXT("Removed %d of %s"), Quantity, *ItemID.ToString());
		return true;
	}
	return false;
}

int32 UInventoryComponent::GetItemCount(const FName ItemID) const
{
	const int32* Count = Items.Find(ItemID);
	return Count ? *Count : 0;
}
