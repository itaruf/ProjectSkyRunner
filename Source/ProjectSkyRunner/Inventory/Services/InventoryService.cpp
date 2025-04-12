// InventoryService.cpp

#pragma once

#include "InventoryService.h"
#include "../Components/InventoryComponent.h"
#include "../../Core/ServiceLocator/GameServiceLocator.h"

UInventoryService::UInventoryService()
{
	// Delegate to the component attached to the player
}

UInventoryService* UInventoryService::CreateInventoryService(UObject* WorldContext)
{
	return NewObject<UInventoryService>(WorldContext);
}

void UInventoryService::InitializeService_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Inventory Service Initialized"));
}

void UInventoryService::ShutdownService_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Inventory Service Shutdown"));
}

void UInventoryService::AddItem(const FName ItemID, const int32 Quantity)
{
	if (UInventoryComponent* InvComp = UGameServiceLocator::GetPlayerInventoryComponent())
	{
		InvComp->AddItem(ItemID, Quantity);
	}
}

bool UInventoryService::RemoveItem(const FName ItemID, const int32 Quantity)
{
	if (UInventoryComponent* InvComp = UGameServiceLocator::GetPlayerInventoryComponent())
	{
		return InvComp->RemoveItem(ItemID, Quantity);
	}
	return false;
}

int32 UInventoryService::GetItemCount(const FName ItemID) const
{
	if (const UInventoryComponent* InvComp = UGameServiceLocator::GetPlayerInventoryComponent())
	{
		return InvComp->GetItemCount(ItemID);
	}
	return 0;
}
