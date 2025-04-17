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

void UGravityCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	Super::PhysFalling(deltaTime, Iterations);
}

void UGravityCharacterMovementComponent::OnLandedInternal(const FHitResult& Hit)
{

}
