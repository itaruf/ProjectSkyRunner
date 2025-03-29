#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitEventComponent.generated.h"

// Declare a dynamic multicast delegate for hit events with the instigating actor and hit location.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitEvent, AActor*, InstigatingActor, FVector, HitLocation);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UHitEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHitEventComponent();

	// Event dispatcher that is broadcast when a hit occurs.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHitEvent OnHit;

	// Call this function to notify listeners about a hit.
	UFUNCTION(BlueprintCallable, Category = "Events")
	void NotifyHit(AActor* InstigatingActor, FVector HitLocation);
};
