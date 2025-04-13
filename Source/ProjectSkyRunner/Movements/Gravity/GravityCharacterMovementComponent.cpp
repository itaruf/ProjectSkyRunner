#include "GravityCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
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

	if (CharacterOwner && CharacterOwner->GetCapsuleComponent())
	{
		CharacterOwner->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UGravityCharacterMovementComponent::OnCharacterHit);
	}
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Landed - Dot with Up: %.2f"), Dot));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Surface Normal: %s"), *LastImpactNormal.ToString()));
	}

	if (bIsDiving)
	{
		bIsDiving = false;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Dive Interrupted - Impact Detected"));
		}

		if (Dot >= FlatSurfaceThreshold)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Flat Surface - Exiting Gravity Mode"));
			}
			ExitGravityMode();
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("Steep Surface - Staying in Gravity Mode"));
			}

			bIsFloating = true;
			GravityScale = 0.f;

			FVector GravityUp = LastImpactNormal.GetSafeNormal();
			FVector NewForward = FVector::CrossProduct(FVector::RightVector, GravityUp).GetSafeNormal();
			FRotator NewRotation = FRotationMatrix::MakeFromXZ(NewForward, GravityUp).Rotator();

			if (CharacterOwner)
			{
				CharacterOwner->SetActorRotation(NewRotation);
			}

			DiveGravityDirection = -GravityUp;
		}
		return;
	}

	if (bIsFloating)
	{
		if (Dot >= FlatSurfaceThreshold)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Floating Hit Flat Surface - Exiting Gravity Mode"));
			}
			ExitGravityMode();
		}
	}
}

void UGravityCharacterMovementComponent::OnCharacterHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsDiving && !bIsFloating) return;
	if (!Hit.bBlockingHit) return;

	LastImpactNormal = Hit.ImpactNormal;
	float Dot = FVector::DotProduct(LastImpactNormal.GetSafeNormal(), FVector::UpVector);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Hit - Dot with Up: %.2f"), Dot));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Impact Normal: %s"), *LastImpactNormal.ToString()));
	}

	if (bIsDiving)
	{
		bIsDiving = false;

		if (Dot >= FlatSurfaceThreshold)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Flat Surface - Exiting Gravity Mode"));

			ExitGravityMode();
			return;
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("Steep Surface - Snapping to surface"));

			bIsFloating = true;
			GravityScale = 0.f;

			FVector GravityUp = LastImpactNormal.GetSafeNormal();
			FVector NewForward = FVector::CrossProduct(FVector::RightVector, GravityUp).GetSafeNormal();
			FRotator NewRotation = FRotationMatrix::MakeFromXZ(NewForward, GravityUp).Rotator();

			if (CharacterOwner)
			{
				CharacterOwner->SetActorRotation(NewRotation);
			}

			DiveGravityDirection = -GravityUp;
			return;
		}
	}

	if (bIsFloating)
	{
		if (Dot >= FlatSurfaceThreshold)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Floating + Flat Impact - Exiting Gravity Mode"));

			ExitGravityMode();
		}
	}
}

