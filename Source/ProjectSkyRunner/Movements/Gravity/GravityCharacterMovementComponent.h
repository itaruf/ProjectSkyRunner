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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity Mode")
	bool bGravityMode;

	// Original gravity scale to restore after toggling gravity mode off
	float OriginalGravityScale;

	// Toggles gravity mode on/off
	UFUNCTION(BlueprintCallable)
	void ToggleGravityMode();

protected:
	virtual void BeginPlay() override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
};
