#pragma once

#include "CoreMinimal.h"
#include "../Enumerations/DamageTypeEx.h"
#include "DamageData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTSKYRUNNER_API FDamageData
{
	GENERATED_BODY()

public:
	// The actor that instigated the damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	AActor* InstigatingActor;

	// The location where the damage occurred.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector HitLocation;

	// The amount of damage dealt.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount;

	// The type of damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageType DamageType;

	// The impulse applied by the hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector HitImpulse;

	// The name of the bone that was hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FName HitBone;

	// The actor that is directly causing the damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	AActor* DamageCauser;

	// Indicates if the hit was a critical hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bCriticalHit;

	// The timestamp when the damage occurred.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float TimeStamp;
};
