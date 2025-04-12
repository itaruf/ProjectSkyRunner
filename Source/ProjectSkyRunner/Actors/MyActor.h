// MyActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class USoundBase;

// This example Actor demonstrates how gameplay classes can access services
// like Inventory or Audio through the Service Locator without direct dependencies
UCLASS()
class PROJECTSKYRUNNER_API AMyActor : public AActor
{
	GENERATED_BODY()

public:
	// Called when the actor is first placed or spawned in the world
	// We'll use this to retrieve and interact with services
	virtual void BeginPlay() override;

	// A sound asset reference that can be set from the editor or Blueprint
	// This sound will be played via the AudioService when the game starts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* MySound;
};