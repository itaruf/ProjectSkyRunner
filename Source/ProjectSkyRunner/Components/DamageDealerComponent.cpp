#include "DamageDealerComponent.h"
#include "../Interfaces/DamageableInterface.h"
#include "GameFramework/Actor.h"

UDamageDealerComponent::UDamageDealerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize default damage values.
	DamageData.DamageAmount = 10.f;
	DamageData.DamageType = EDamageType::Physical;
	DamageData.HitImpulse = FVector::ZeroVector;
	DamageData.HitBone = NAME_None;
	DamageData.bCriticalHit = false;
	// InstigatingActor, HitLocation, DamageCauser, and TimeStamp will be set at runtime.
}

void UDamageDealerComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UDamageDealerComponent::CalculateDamage(const FDamageData& InDamageData, AActor* Target) const
{
	// Base implementation: simply return the base damage.
	return InDamageData.DamageAmount;
}

void UDamageDealerComponent::DealDamage(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	// Check if the target implements the damageable interface.
	if (Target->Implements<UDamageableInterface>())
	{
		// Build the damage data with runtime details.
		FDamageData DamageToApply = DamageData;
		DamageToApply.InstigatingActor = GetOwner();
		DamageToApply.HitLocation = Target->GetActorLocation();
		DamageToApply.DamageCauser = GetOwner();
		DamageToApply.TimeStamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

		// Calculate final damage (may be modified by derived classes).
		float FinalDamage = CalculateDamage(DamageToApply, Target);

		// Apply damage using the damageable interface.
		IDamageableInterface::Execute_TakeDamage(Target, FinalDamage);
	}
}
