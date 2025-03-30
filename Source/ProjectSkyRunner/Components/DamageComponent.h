#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Structures/DamageData.h"
#include "DamageComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageComponent();

	// Default damage values stored in a structured format.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FDamageData DamageData;

	// Deals damage to the specified target.
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor* Target);

protected:
	virtual void BeginPlay() override;
};

