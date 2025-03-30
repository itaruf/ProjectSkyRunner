#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Structures/DamageData.h"
#include "HitEventComponent.generated.h"

// Declare a dynamic multicast delegate that uses DamageData as its parameter.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitEvent, FDamageData, DamageData);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UHitEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:    
	UHitEventComponent();

	// Event dispatcher that is broadcast when a hit occurs.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHitEvent OnHit;

	// Call this function to notify listeners about a hit, including the damage details.
	UFUNCTION(BlueprintCallable, Category = "Events")
	void NotifyHit(AActor* InstigatingActor, FVector HitLocation, float DamageAmount);
};
