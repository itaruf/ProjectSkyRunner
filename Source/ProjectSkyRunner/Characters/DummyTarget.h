#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/DamageableInterface.h"
#include "DummyTarget.generated.h"

UCLASS()
class PROJECTSKYRUNNER_API ADummyTarget : public AActor, public IDamageableInterface
{
	GENERATED_BODY()
	
public:	
	ADummyTarget();

protected:
	virtual void BeginPlay() override;

	// Visual representation of the dummy target
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* TargetMesh;

	// Health stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float CurrentHealth;

public:	
	virtual void Tick(float DeltaTime) override;

	// --- IDamageableInterface Methods ---
	virtual void TakeDamage_Implementation(float DamageAmount) override;
	virtual float GetCurrentHealth_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;
	virtual bool IsDead_Implementation() const override;

private:
	void HandleDeath();
};
