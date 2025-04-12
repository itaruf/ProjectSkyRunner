// GameServiceInterface.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameServiceInterface.generated.h"

// UGameServiceInterface is the base interface used to define all service types
// that will be managed by the Service Locator
// This interface is marked as Blueprintable to allow for Blueprint-based services
// if needed in more designer-oriented workflows
UINTERFACE(Blueprintable)
class PROJECTSKYRUNNER_API UGameServiceInterface : public UInterface
{
	GENERATED_BODY()
};

// The IGameServiceInterface interface defines a standard lifecycle contract
// that all services must follow. This ensures consistency across systems
// when they are initialized and cleaned up by the Service Locator
class PROJECTSKYRUNNER_API IGameServiceInterface
{
	GENERATED_BODY()

public:
	// Called when the service is initialized (e.g., when registered)
	// Exposed to Blueprints as a native event.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Service")
	void InitializeService();
	virtual void InitializeService_Implementation() = 0;

	// Called when the service is shut down (e.g., when unregistered or game ends)
	// Exposed to Blueprints as a native event.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Service")
	void ShutdownService();
	virtual void ShutdownService_Implementation() = 0;
};
