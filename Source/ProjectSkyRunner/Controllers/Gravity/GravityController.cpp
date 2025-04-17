#include "GravityController.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Movements/Gravity/GravityCharacterMovementComponent.h"

void AGravityController::BeginPlay()
{
	Super::BeginPlay();

	GravityCharacter = Cast<ACharacter>(GetPawn());
	if (GravityCharacter)
	{
		GravityCharacterMovement = Cast<UGravityCharacterMovementComponent>(GravityCharacter->GetCharacterMovement());
	}
}

void AGravityController::UpdateRotation(float DeltaTime)
{
	const FVector GravityDirection = GravityCharacterMovement->GetGravityDirection();

	// Get the current control rotation in world space
	FRotator ViewRotation = GetControlRotation();

	// Add any rotation from the gravity changes, if any happened.
	// Delete this code block if you don't want the camera to automatically compensate for gravity rotation.
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, GravityDirection);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);

		ViewRotation = WarpedCameraRotation.Rotator();
	}
	LastFrameGravity = GravityDirection;

	// Convert the view rotation from world space to gravity relative space.
	// Now we can work with the rotation as if no custom gravity was affecting it.
	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);

	// Calculate Delta to be applied on ViewRotation
	FRotator DeltaRot(RotationInput);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);

		// Zero the roll of the camera as we always want it horizontal in relation to the gravity.
		ViewRotation.Roll = 0;

		// Convert the rotation back to world space, and set it as the current control rotation.
		SetControlRotation(GetGravityWorldRotation(ViewRotation, GravityDirection));
	}

	GravityCharacter->FaceRotation(ViewRotation, DeltaTime);
}

FRotator AGravityController::GetGravityRelativeRotation(const FRotator& Rotation, const FVector& GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		const FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

FRotator AGravityController::GetGravityWorldRotation(const FRotator& Rotation, const FVector& GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		const FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

void AGravityController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (GravityShiftAction)
		{
			EnhancedInputComponent->BindAction(GravityShiftAction, ETriggerEvent::Started, this, &AGravityController::GravityShift);
		}
		if (GravityReturnAction)
		{
			EnhancedInputComponent->BindAction(GravityReturnAction, ETriggerEvent::Started, this, &AGravityController::GravityReturn);
		}
		if (GravityDiveAction)
		{
			EnhancedInputComponent->BindAction(GravityDiveAction, ETriggerEvent::Started, this, &AGravityController::GravityDive);
		}
	}
}

void AGravityController::GravityShift()
{
	if (GravityCharacter)
	{
		if (GravityCharacterMovement)
		{
			UE_LOG(LogTemp, Warning, TEXT("GravityShift: Restored normal gravity"));
		}
	}
}

void AGravityController::GravityReturn()
{
	if (GravityCharacter)
	{
		if (GravityCharacterMovement)
		{
			UE_LOG(LogTemp, Warning, TEXT("GravityReturn: Restored normal gravity"));
		}
	}
}

void AGravityController::GravityDive()
{
	if (GravityCharacter)
	{
		if (GravityCharacterMovement)
		{
			if (GravityCharacterMovement->bIsFloating && !GravityCharacterMovement->bIsDiving)
			{
				UE_LOG(LogTemp, Warning, TEXT("GravityDive: Starting dive"));
			}
		}
	}
}
