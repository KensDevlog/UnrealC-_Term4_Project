// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"

#include "NiagaraFunctionLibrary.h"
#include "SpellGameCharacter.h"
#include "Engine/DamageEvents.h"


void USpell::CastSpell(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction) const
{
	if (!ProjectileClass || !Instigator) return;
	UWorld* World = Instigator->GetWorld();
	if (!World) return;
	
	switch (FiringMode)
	{
	case EFiringMode::Projectile:
		CastSpell_Projectile(Instigator, Causer, Origin, Direction, World);
		break;
	case EFiringMode::Hitscan:
		CastSpell_Hitscan(Instigator, Causer, Origin, Direction, World);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Firing Mode"));
	}

	// "Muzzle Flash" but for spells... spell flash? idk its just called CastEffect so yeah
	if (CastEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, CastEffect, Origin, Direction);
	}
}

void USpell::CastSpell_Projectile(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction, UWorld* World) const
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn>(Instigator);
	SpawnParams.Owner = Causer;

	ASpellProjectile* Projectile = World->SpawnActor<ASpellProjectile>(
		ProjectileClass,
		Origin,
		Direction,
		SpawnParams
	);

	if (Projectile)
	{
		Projectile->Damage = Damage;
		Projectile->InstigatorController = Instigator->GetInstigatorController();
	}
}

void USpell::CastSpell_Hitscan(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction, UWorld* World) const
{
	if (!Instigator) return;
	if (!World) return;
	
	FVector Start = Origin;
	FVector End = Start + Direction.Vector() * WORLD_MAX;
	
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);
	
	bool bHit = World->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_WorldDynamic,
		QueryParams
		);
	
	if (bHit)
	{
		if (ASpellGameCharacter* HitCharacter = Cast<ASpellGameCharacter>(Hit.GetActor()))
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			DamageEvent.ShotDirection = Direction.Vector();
			DamageEvent.DamageTypeClass = UDamageType::StaticClass();
			
			HitCharacter->TakeDamage(Damage, DamageEvent, Instigator->GetInstigatorController(), Instigator);
		}
		if (HitscanHitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				World,
				HitscanHitEffect,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation());
		}
	}
}
