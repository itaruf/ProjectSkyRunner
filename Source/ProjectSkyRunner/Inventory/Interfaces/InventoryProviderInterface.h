#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Components/InventoryComponent.h"
#include "InventoryProviderInterface.generated.h"

UINTERFACE(Blueprintable)
class PROJECTSKYRUNNER_API UInventoryProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for providing a InventoryComponent
 */
class PROJECTSKYRUNNER_API IInventoryProviderInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the InventoryComponent associated with this actor
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const;
};
