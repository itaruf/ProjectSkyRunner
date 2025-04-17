#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../Movements/Gravity/GravityCharacterMovementComponent.h"
#include "../../Controllers/Interfaces/ControllerInterface.h"
#include "GravityController.generated.h"

class UInputAction;
struct FInputActionValue;
/**
 * A Player Controller class which adds input-handling functionality for
 * CharacterMovementController's custom gravity mechanics.
 */
UCLASS()
class AGravityController : public APlayerController, public IControllerInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GravityShiftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GravityReturnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GravityDiveAction;
	
public:
	virtual void BeginPlay() override;
	
	virtual void UpdateRotation(float DeltaTime) override;

	// Converts a rotation from world space to gravity relative space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(const FRotator& Rotation, const FVector& GravityDirection);

	// Converts a rotation from gravity relative space to world space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(const FRotator& Rotation, const FVector& GravityDirection);

	// --- New functions to control gravity actions ---
	UFUNCTION(BlueprintCallable)
	void GravityShift();

	UFUNCTION(BlueprintCallable)
	void GravityReturn();

	UFUNCTION(BlueprintCallable)
	void GravityDive();
private:
	//
	FVector LastFrameGravity = FVector::ZeroVector;
	//
	UPROPERTY()
	ACharacter* GravityCharacter = nullptr;
	//
	UPROPERTY()
	UGravityCharacterMovementComponent* GravityCharacterMovement = nullptr;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
