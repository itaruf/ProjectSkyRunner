#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for any object that can take damage.
 */
class PROJECTSKYRUNNER_API IDamageableInterface
{
	GENERATED_BODY()

public:
	/** Called to apply damage to the object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void TakeDamage(float DamageAmount);

	/** Returns the current health value */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetCurrentHealth() const;

	/** Returns the maximum health value */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetMaxHealth() const;

	/** Returns true if the object is dead (health is 0 or below) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	bool IsDead() const;
};
