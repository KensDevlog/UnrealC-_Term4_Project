// Fill out your copyright notice in the Description page of Project Settings.


#include "WandComponent.h"

#include "Spell.h"
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
	
	CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
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
	
	ShootSpell();
}

void UWandComponent::ShootSpell()
{
	FVector SpellSpawnPoint = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f;
	
	FRotator ShootDir;
	CameraComponent ?  ShootDir = CameraComponent->GetComponentRotation() : ShootDir = GetOwner()->GetActorRotation();
	
	EquippedSpell->CastSpell(GetOwner(), GetOwner(), SpellSpawnPoint, ShootDir);
}

void UWandComponent::HandleShootInput(bool InputDown)
{
	InputDown ? IsTryingToCast = true : IsTryingToCast = false;
}

