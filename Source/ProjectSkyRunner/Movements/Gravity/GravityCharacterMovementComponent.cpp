#include "GravityCharacterMovementComponent.h"
#include "GameFramework/Character.h"

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
	if (bIsDiving)
	{
		bIsDiving = false;
		bIsFloating = true;
		Velocity = FVector::ZeroVector;
		GravityScale = 0.f;
		SetMovementMode(MOVE_Falling);
		return;
	}

	if (!bIsFloating)
	{
		if (CharacterOwner && CharacterOwner->GetMovementComponent()->IsMovingOnGround())
		{
			FVector Impulse = FVector(0.f, 0.f, LiftImpulse);
			AddImpulse(Impulse);
			Velocity.Z = LiftImpulse;
		}
		bIsFloating = true;
		GravityScale = 0.f;
		SetMovementMode(MOVE_Falling);
	}
}

void UGravityCharacterMovementComponent::ExitGravityMode()
{
	bIsFloating = false;
	bIsDiving = false;
	GravityScale = OriginalGravityScale;
	if (CharacterOwner && CharacterOwner->GetMovementComponent()->IsMovingOnGround())
	{
		SetMovementMode(MOVE_Walking);
	}
	else
	{
		SetMovementMode(MOVE_Falling);
	}
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

void UGravityCharacterMovementComponent::OnLandedInternal(const FHitResult& Hit)
{
	LastImpactNormal = Hit.ImpactNormal;
	float Dot = FVector::DotProduct(LastImpactNormal.GetSafeNormal(), FVector::UpVector);

	if (bIsDiving)
	{
		if (Dot >= FlatSurfaceThreshold)
		{
			ExitGravityMode();
		}
		else
		{
			bIsDiving = false;
			bIsFloating = true;
			GravityScale = 0.f;
		}
		return;
	}

	if (bIsFloating)
	{
		if (Dot >= FlatSurfaceThreshold)
		{
			ExitGravityMode();
		}
	}
}
