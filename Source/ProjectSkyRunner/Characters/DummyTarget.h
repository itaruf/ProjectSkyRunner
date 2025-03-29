#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/DamageableInterface.h"
#include "../Interfaces/HitEventProviderInterface.h" // Include the hit event provider interface
#include "DummyTarget.generated.h"

UCLASS()
class PROJECTSKYRUNNER_API ADummyTarget : public AActor, public IDamageableInterface, public IHitEventProviderInterface
{
	GENERATED_BODY()
	
public:	
	ADummyTarget();

protected:
	virtual void BeginPlay() override;

	// Capsule component for collision
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	// Skeletal mesh component for visuals and animation
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent* DummyMesh;

	// Health stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float CurrentHealth;

	// Hit event component to broadcast hit events
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UHitEventComponent* HitEventComp;

public:	
	virtual void Tick(float DeltaTime) override;

	// --- IDamageableInterface Methods ---
	virtual void TakeDamage_Implementation(float DamageAmount) override;
	virtual float GetCurrentHealth_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;
	virtual bool IsDead_Implementation() const override;

	// --- IHitEventProviderInterface Methods ---
	virtual UHitEventComponent* GetHitEventComponent_Implementation() const override;

private:
	void HandleDeath();
};
