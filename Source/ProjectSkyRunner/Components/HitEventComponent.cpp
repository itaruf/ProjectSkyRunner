// HitEventComponent.cpp
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
    
	// Initialize additional fields with default values or computed data.
	DamageData.DamageType = EDamageType::Physical;
	DamageData.HitImpulse = FVector::ZeroVector;
	DamageData.HitBone = NAME_None;
	DamageData.DamageCauser = InstigatingActor;
	DamageData.bCriticalHit = false;
	DamageData.TimeStamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

	// Broadcast the hit event with the complete damage data.
	OnHit.Broadcast(DamageData);
}
