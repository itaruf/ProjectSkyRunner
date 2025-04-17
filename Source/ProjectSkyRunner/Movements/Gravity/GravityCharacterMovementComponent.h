#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GravityCharacterMovementComponent.generated.h"

UCLASS()
class PROJECTSKYRUNNER_API UGravityCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UGravityCharacterMovementComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity|Mode")
	bool bGravityMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity|Mode")
	float OriginalGravityScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity|Mode")
	bool bIsFloating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity|Mode")
	bool bIsDiving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity|Float")
	float LiftImpulse = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity|Dive")
	FVector DiveGravityDirection = FVector(0, 0, -1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity|Dive")
	float DiveGravityMultiplier = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity|Dive")
	float DiveSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity|Dive")
	float MaxDiveDuration = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity|Dive")
	float DiveStartTime = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity|Landing")
	FVector LastImpactNormal;
	
	void OnLandedInternal(const FHitResult& Hit);
protected:
	virtual void BeginPlay() override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;
};
