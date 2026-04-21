// Ken Arigo @ VFS 2026


#include "WandComponent.h"

#include "Spell.h"
#include "SpellGameCharacter.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
UWandComponent::UWandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWandComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EquippedSpellClass)
	{
		EquippedSpell = NewObject<USpell>(this, EquippedSpellClass);
	}
}


// Called every frame
void UWandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsTryingToCast || !EquippedSpell) return;
	if (GetWorld()->GetTimeSeconds() < NextPrimaryIntervalShotTime) return;
	
	if (GetWorld()->GetTimeSeconds() < NextSecondaryIntervalShotTime) return;
	
	++BurstShotIteration;
	
	if (BurstShotIteration >= EquippedSpell->ShotsPerBurst)
	{
		NextPrimaryIntervalShotTime = GetWorld()->GetTimeSeconds() + EquippedSpell->PrimaryShotInterval;
		BurstShotIteration = 0;
	}
	else
	{
		NextSecondaryIntervalShotTime = GetWorld()->GetTimeSeconds() + EquippedSpell->SecondaryShotInterval;
	}
	
	if (ASpellGameCharacter* Owner = Cast<ASpellGameCharacter>(GetOwner()))
	{
		FVector SpellSpawnPoint = Owner->CameraComponent->GetComponentLocation() + Owner->CameraComponent->GetForwardVector() * 100.f;
		FRotator ShootDir = Owner->CameraComponent->GetComponentRotation();
		ShootSpell(SpellSpawnPoint, ShootDir);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner invalid!"));
	}	
	
}

void UWandComponent::ShootSpell_Implementation(FVector SpellSpawnPoint, FRotator ShootDir)
{
	EquippedSpell->CastSpell(GetOwner(), GetOwner(), SpellSpawnPoint, ShootDir);
}

void UWandComponent::HandleShootInput(bool InputDown)
{
	InputDown ? IsTryingToCast = true : IsTryingToCast = false;
}

USpell* UWandComponent::GetCurrentSpell()
{
	return EquippedSpell;
}

void UWandComponent::ChangeEquippedSpell_Implementation(USpell* NewSpell)
{
	EquippedSpell = NewSpell;
}

void UWandComponent::ChangeEquippedSpellByClass_Implementation(TSubclassOf<USpell> NewSpellClass)
{
	if (!NewSpellClass) return;

	// Spawn de la nueva instancia
	USpell* NewSpell = NewObject<USpell>(GetOwner(), NewSpellClass);

	// Guardar la clase y llamar al sistema existente
	EquippedSpellClass = NewSpellClass;
	ChangeEquippedSpell(NewSpell);
}

TSubclassOf<USpell> UWandComponent::GetCurrentSpellClass()
{
	return EquippedSpellClass;
}

