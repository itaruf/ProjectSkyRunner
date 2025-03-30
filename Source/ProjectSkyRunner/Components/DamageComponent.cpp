#include "DamageComponent.h"
#include "../Interfaces/DamageableInterface.h"
#include "GameFramework/Actor.h"

UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Set default damage values.
	DamageData.DamageAmount = 10.f;
	DamageData.DamageType = EDamageType::Physical;
	DamageData.HitImpulse = FVector::ZeroVector;
	DamageData.HitBone = NAME_None;
	DamageData.bCriticalHit = false;
	// InstigatingActor, HitLocation, DamageCauser, and TimeStamp will be set at runtime.
}

void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDamageComponent::DealDamage(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	// Check if the target implements the damageable interface.
	if (Target->Implements<UDamageableInterface>())
	{
		// Create a copy of the default damage data and fill in runtime details.
		FDamageData DamageToApply = DamageData;
		DamageToApply.InstigatingActor = GetOwner();
		DamageToApply.HitLocation = Target->GetActorLocation();
		DamageToApply.DamageCauser = GetOwner();
		DamageToApply.TimeStamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

		// Apply damage using the damage amount from the structure.
		IDamageableInterface::Execute_TakeDamage(Target, DamageToApply.DamageAmount);
	}
}
