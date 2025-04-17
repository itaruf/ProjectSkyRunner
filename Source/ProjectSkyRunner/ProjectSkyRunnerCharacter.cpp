#include "ProjectSkyRunnerCharacter.h"
#include "Movements/Gravity/GravityCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DamageDealerComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectSkyRunnerCharacter::AProjectSkyRunnerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	DamageDealerComp = CreateDefaultSubobject<UDamageDealerComponent>(TEXT("DamageDealerComp"));
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
}

void AProjectSkyRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AProjectSkyRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectSkyRunnerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectSkyRunnerCharacter::Look);
		if (GravityShiftAction)
		{
			EnhancedInputComponent->BindAction(GravityShiftAction, ETriggerEvent::Triggered, this, &AProjectSkyRunnerCharacter::GravityShift);
		}
		if (GravityReturnAction)
		{
			EnhancedInputComponent->BindAction(GravityReturnAction, ETriggerEvent::Triggered, this, &AProjectSkyRunnerCharacter::GravityReturn);
		}
		if (GravityDiveAction)
		{
			EnhancedInputComponent->BindAction(GravityDiveAction, ETriggerEvent::Triggered, this, &AProjectSkyRunnerCharacter::GravityDive);
		}
	}
}

void AProjectSkyRunnerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		auto GravityComp = Cast<UGravityCharacterMovementComponent>(GetCharacterMovement());
		if (GravityComp && GravityComp->bIsFloating)
		{
			return;
		}
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProjectSkyRunnerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectSkyRunnerCharacter::GravityShift()
{
	if (auto GravityComp = Cast<UGravityCharacterMovementComponent>(GetCharacterMovement()))
	{
		GravityComp->HandleGravityShift();
	}
}

void AProjectSkyRunnerCharacter::GravityReturn()
{
	if (auto GravityComp = Cast<UGravityCharacterMovementComponent>(GetCharacterMovement()))
	{
		GravityComp->ExitGravityMode();
	}
}

void AProjectSkyRunnerCharacter::GravityDive()
{
	if (auto GravityComp = Cast<UGravityCharacterMovementComponent>(GetCharacterMovement()))
	{
		GravityComp->StartDive(GetFollowCamera()->GetForwardVector());
	}
}

void AProjectSkyRunnerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (auto GravityComp = Cast<UGravityCharacterMovementComponent>(GetCharacterMovement()))
	{
		GravityComp->OnLandedInternal(Hit);
	}
}

UDamageDealerComponent* AProjectSkyRunnerCharacter::GetDamageDealerComponent_Implementation() const
{
	return DamageDealerComp;
}

UInventoryComponent* AProjectSkyRunnerCharacter::GetInventoryComponent_Implementation() const
{
	return InventoryComp;
}
