#include "GravityCharacterMovementComponent.h"

UGravityCharacterMovementComponent::UGravityCharacterMovementComponent()
{
	bGravityMode = false;
}

void UGravityCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalGravityScale = GravityScale;  // Store original gravity scale
}

void UGravityCharacterMovementComponent::ToggleGravityMode()
{
	bGravityMode = !bGravityMode;

	if (bGravityMode)
	{
		GravityScale = 0.f;              // Disable gravity
		Velocity = FVector::ZeroVector;  // Immediately stop movement
		SetMovementMode(MOVE_Falling);   // Ensure we're in falling mode to allow floating
	}
	else
	{
		GravityScale = OriginalGravityScale;  // Restore gravity
		SetMovementMode(MOVE_Falling);        // Return to standard falling mode
	}
}

void UGravityCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	if (bGravityMode)
	{
		Velocity = FVector::ZeroVector; // Completely stationary when in gravity mode
		return;                         // Skip default gravity physics
	}

	Super::PhysFalling(deltaTime, Iterations);
}
