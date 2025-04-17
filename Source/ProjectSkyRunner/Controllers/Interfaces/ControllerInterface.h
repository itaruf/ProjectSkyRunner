#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

class UInputComponent;

UINTERFACE(BlueprintType)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to share core functionalities between controllers
 */
class PROJECTSKYRUNNER_API IControllerInterface
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) = 0;
};
