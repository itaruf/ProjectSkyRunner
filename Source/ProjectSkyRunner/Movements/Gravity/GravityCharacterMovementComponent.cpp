#include "GravityCharacterMovementComponent.h"

UGravityCharacterMovementComponent::UGravityCharacterMovementComponent()
{
	bGravityMode = false;
	bIsFloating = false;
	bIsDiving = false;
}

void UGravityCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OriginalGravityScale = GravityScale;
}

void UGravityCharacterMovementComponent::HandleGravityShift()
{
	if (!bIsFloating && !bIsDiving)
	{
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
	GravityScale = OriginalGravityScale;
	SetMovementMode(MOVE_Falling);
}

void UGravityCharacterMovementComponent::StartDive(const FVector& DiveDirection)
{
	if (bIsFloating && !bIsDiving)
	{
		bIsFloating = false;
		bIsDiving = true;
		Velocity = DiveDirection.GetSafeNormal() * DiveSpeed;
		SetMovementMode(MOVE_Falling);
		DiveStartTime = GetWorld()->GetTimeSeconds();
	}
}

void UGravityCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	if (bIsFloating)
	{
		Velocity = FVector::ZeroVector;
		return;
	}
	if (bIsDiving)
	{
		float ElapsedTime = GetWorld()->GetTimeSeconds() - DiveStartTime;
		if (ElapsedTime >= MaxDiveDuration)
		{
			ExitGravityMode();
			return;
		}
		FVector GravityForce = DiveGravityDirection * GetWorld()->GetGravityZ() * DiveGravityMultiplier;
		Velocity += GravityForce * deltaTime;
		if (IsMovingOnGround())
		{
			ExitGravityMode();
			return;
		}
		Super::PhysFalling(deltaTime, Iterations);
		return;
	}
	Super::PhysFalling(deltaTime, Iterations);
}
