// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"

#include "NiagaraFunctionLibrary.h"


void USpell::CastSpell(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction) const
{
	if (!ProjectileClass || !Instigator) return;

	UWorld* World = Instigator->GetWorld();
	if (!World) return;

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

	if (CastEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, CastEffect, Origin, Direction);
	}
}
