#include "GravityCharacterMovementComponent.h"

UGravityCharacterMovementComponent::UGravityCharacterMovementComponent()
{
	bGravityMode = false;
}

void UGravityCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalGravityScale = GravityScale; // Store original gravity scale
}

void UGravityCharacterMovementComponent::HandleGravityShift()
{
	if (!bIsFloating && !bIsDiving)
	{
		// Enter float mode
		bIsFloating = true;
		GravityScale = 0.f;
		Velocity = FVector::ZeroVector;
		SetMovementMode(MOVE_Falling);
	}
}

void UGravityCharacterMovementComponent::ExitGravityMode()
{
	bIsFloating = false;
	bIsDiving = false;
	GravityScale = OriginalGravityScale; // Restore original gravity
	SetMovementMode(MOVE_Falling); // Allow normal gravity behavior
}

void UGravityCharacterMovementComponent::StartDive(const FVector& DiveDirection)
{
	if (bIsFloating && !bIsDiving)
	{
		bIsFloating = false;
		bIsDiving = true;

		// Set initial dive velocity, adjust speed as needed
		Velocity = DiveDirection.GetSafeNormal() * DiveSpeed;

		SetMovementMode(MOVE_Falling);
	}
}

void UGravityCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	if (bIsFloating)
	{
		Velocity = FVector::ZeroVector; // Stay perfectly floating
		return; // Skip gravity physics entirely
	}

	if (bIsDiving)
	{
		// Constrain slight directional adjustments (future implementation)
		const float GravityMagnitude = FMath::Abs(GetWorld()->GetGravityZ());
		FVector GravityForce = DiveGravityDirection * GravityMagnitude * DiveGravityMultiplier;
		Velocity += GravityForce * deltaTime;

		// Optional: Allow limited directional input adjustment here (future)

		Super::PhysFalling(deltaTime, Iterations);
		return;
	}

	// Standard falling physics
	Super::PhysFalling(deltaTime, Iterations);
}
