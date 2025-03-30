#include "MeleeDamageDealerComponent.h"
#include "GameFramework/Actor.h"

UMeleeDamageDealerComponent::UMeleeDamageDealerComponent()
{
	// Set default melee multiplier.
	MeleeMultiplier = 1.5f;
}

float UMeleeDamageDealerComponent::CalculateDamage(const FDamageData& InDamageData, AActor* Target) const
{
	// Multiply the base damage by the melee multiplier.
	return InDamageData.DamageAmount * MeleeMultiplier;
}
