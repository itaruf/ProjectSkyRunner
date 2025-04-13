#include "GravityCharacterMovementComponent.h"

#include "GameFramework/Character.h"

UGravityCharacterMovementComponent::UGravityCharacterMovementComponent()
{
	bGravityMode = false;
}

void UGravityCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalGravityScale = GravityScale;  // Store original gravity scale
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
	else if (bIsFloating && !bIsDiving)
	{
		// Begin diving towards aim direction (camera forward vector)
		if (CharacterOwner && CharacterOwner->Controller)
		{
			bIsDiving = true;
			bIsFloating = false;
		}
	}
}

void UGravityCharacterMovementComponent::ExitGravityMode()
{
	bIsFloating = false;
	bIsDiving = false;
	GravityScale = OriginalGravityScale; // Restore original gravity
	SetMovementMode(MOVE_Falling);       // Allow normal gravity behavior
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
		FVector GravityForce = FVector(0, 0, -1) * GravityMagnitude * 0.1f; // small gravity to slightly curve downwards
		Velocity += GravityForce * deltaTime;

		// Optional: Allow limited directional input adjustment here (future)

		return; // Skip default gravity
	}

	// Standard falling physics
	Super::PhysFalling(deltaTime, Iterations);
}
