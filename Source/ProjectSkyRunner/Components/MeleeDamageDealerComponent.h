#pragma once

#include "CoreMinimal.h"
#include "DamageDealerComponent.h"
#include "MeleeDamageDealerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UMeleeDamageDealerComponent : public UDamageDealerComponent
{
	GENERATED_BODY()

public:
	UMeleeDamageDealerComponent();

protected:
	// Override to apply a melee-specific damage multiplier.
	virtual float CalculateDamage(const FDamageData& InDamageData, AActor* Target) const override;

	// Multiplier to scale melee damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage|Melee")
	float MeleeMultiplier;
};
