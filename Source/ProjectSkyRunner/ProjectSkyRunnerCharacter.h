#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageDealerProviderInterface.h"
#include "Inventory/Interfaces/InventoryProviderInterface.h"
#include "Logging/LogMacros.h"
#include "ProjectSkyRunnerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProjectSkyRunnerCharacter : public ACharacter, public IDamageDealerProviderInterface, public IInventoryProviderInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AProjectSkyRunnerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDamageDealerComponent* DamageDealerComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UInventoryComponent* InventoryComp;

	virtual UDamageDealerComponent* GetDamageDealerComponent_Implementation() const override;
	virtual UInventoryComponent* GetInventoryComponent_Implementation() const override;
};
