#include "HitEventComponent.h"

UHitEventComponent::UHitEventComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHitEventComponent::NotifyHit(AActor* InstigatingActor, FVector HitLocation)
{
	OnHit.Broadcast(InstigatingActor, HitLocation);
}
