#include "HitEventComponent.h"

UHitEventComponent::UHitEventComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHitEventComponent::NotifyHit(AActor* InstigatingActor, FVector HitLocation, float DamageAmount)
{
	// Build the damage data struct.
	FDamageData DamageData;
	DamageData.InstigatingActor = InstigatingActor;
	DamageData.HitLocation = HitLocation;
	DamageData.DamageAmount = DamageAmount;

	// Broadcast the hit event with the new data.
	OnHit.Broadcast(DamageData);
}
