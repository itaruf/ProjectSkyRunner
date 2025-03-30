#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Structures/DamageData.h"

#include "DamageDealerComponent.generated.h"  // MUST be the last include

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSKYRUNNER_API UDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageDealerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FDamageData DamageData;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor* Target);

protected:
	virtual void BeginPlay() override;
};
