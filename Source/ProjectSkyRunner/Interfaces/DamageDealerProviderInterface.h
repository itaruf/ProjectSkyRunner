#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Components/DamageDealerComponent.h"
#include "DamageDealerProviderInterface.generated.h"

UINTERFACE(Blueprintable)
class PROJECTSKYRUNNER_API UDamageDealerProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for providing a DamageComponent.
 */
class PROJECTSKYRUNNER_API IDamageDealerProviderInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the DamageComponent associated with this actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	UDamageDealerComponent* GetDamageDealerComponent() const;
};
