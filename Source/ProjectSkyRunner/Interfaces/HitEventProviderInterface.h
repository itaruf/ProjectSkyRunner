#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Components/HitEventComponent.h" // Include the component header so it is available in the interface
#include "HitEventProviderInterface.generated.h"

UINTERFACE(Blueprintable)
class PROJECTSKYRUNNER_API UHitEventProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for providing a HitEventComponent.
 */
class PROJECTSKYRUNNER_API IHitEventProviderInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the HitEventComponent associated with this actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hit")
	UHitEventComponent* GetHitEventComponent() const;
};
