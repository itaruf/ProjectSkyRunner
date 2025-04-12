// InventoryComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// Inventory operations
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItem(const FName ItemID, const int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(const FName ItemID, const int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	int32 GetItemCount(const FName ItemID) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TMap<FName, int32> Items;
};
