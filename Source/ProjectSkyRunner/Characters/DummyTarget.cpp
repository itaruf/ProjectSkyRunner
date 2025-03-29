#include "DummyTarget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

ADummyTarget::ADummyTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set default health values
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	// Create and setup the skeletal mesh component
	DummyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DummyMesh"));
	RootComponent = DummyMesh;
	DummyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ADummyTarget::BeginPlay()
{
	Super::BeginPlay();
}

void ADummyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// --- IDamageableInterface Methods Implementation ---

void ADummyTarget::TakeDamage_Implementation(float DamageAmount)
{
	if (IsDead())
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("DummyTarget took damage: %f | Current Health: %f"), DamageAmount, CurrentHealth);

	if (IsDead())
	{
		HandleDeath();
	}
}

float ADummyTarget::GetCurrentHealth_Implementation() const
{
	return CurrentHealth;
}

float ADummyTarget::GetMaxHealth_Implementation() const
{
	return MaxHealth;
}

bool ADummyTarget::IsDead_Implementation() const
{
	return CurrentHealth <= 0.0f;
}

void ADummyTarget::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("DummyTarget is dead!"));

	// Optionally, play a death animation here via DummyMesh->GetAnimInstance()

	Destroy();
}
