#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GravityCharacterMovementComponent.generated.h"

/**
 * Custom movement component for gravity toggle (floating) mode.
 */
UCLASS()
class PROJECTSKYRUNNER_API UGravityCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UGravityCharacterMovementComponent();

	// Indicates whether gravity mode (floating) is active
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity|Mode")
	bool bGravityMode;

	// Original gravity scale to restore after toggling gravity mode off
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity|Mode")
	float OriginalGravityScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity|Mode")
	bool bIsFloating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity|Mode")
	bool bIsDiving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity|Dive")
	FVector DiveGravityDirection = FVector(0, 0, -1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity|Dive")
	float DiveGravityMultiplier = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity|Dive")
	float DiveSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity|Dive")
	float MaxDiveDuration = 2.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity|Dive")
	float DiveStartTime = 0.f;

	void HandleGravityShift(); // R1
	void ExitGravityMode(); // L1
	void StartDive(const FVector& DiveDirection); // To be defined

protected:
	virtual void BeginPlay() override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
};
