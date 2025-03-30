#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Structures/DamageData.h"
#include "DamageDealerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageDealerComponent();

	// Default damage configuration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FDamageData DamageData;

	// Deals damage to a specified target.
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void DealDamage(AActor* Target);

protected:
	virtual void BeginPlay() override;

	// Virtual function to calculate the final damage value.
	virtual float CalculateDamage(const FDamageData& InDamageData, AActor* Target) const;
};
