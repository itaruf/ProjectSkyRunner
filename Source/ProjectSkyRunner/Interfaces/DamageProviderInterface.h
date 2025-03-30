#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Components/DamageComponent.h"
#include "DamageProviderInterface.generated.h"

UINTERFACE(Blueprintable)
class PROJECTSKYRUNNER_API UDamageProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for providing a DamageComponent.
 */
class PROJECTSKYRUNNER_API IDamageProviderInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the DamageComponent associated with this actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	UDamageComponent* GetDamageComponent() const;
};
