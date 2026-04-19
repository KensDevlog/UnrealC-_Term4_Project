// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NiagaraSystem.h"
#include "SpellProjectile.h"
#include "Spell.generated.h"


/**
 * 
 */

class USphereComponent;

UENUM()
enum class EFiringMode
{
	None,
	Projectile,
	Hitscan
};

UCLASS(Blueprintable, BlueprintType)
class TERM4_FINAL_API USpell : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EFiringMode FiringMode = EFiringMode::Projectile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> CastEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> HitscanHitEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PrimaryShotInterval = 1;
	
	// For burst shots, leave at 0 if regular auto
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SecondaryShotInterval = 0;
	
	// For burst shots, leave at 1 for regular auto
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int ShotsPerBurst = 1;
	
	UFUNCTION(BlueprintCallable)
	void CastSpell(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction) const;
	
	UFUNCTION()
	void CastSpell_Projectile(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction, UWorld* World) const;
	
	UFUNCTION()
	void CastSpell_Hitscan(AActor* Instigator, AActor* Causer, const FVector& Origin, const FRotator& Direction, UWorld* World) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASpellProjectile> ProjectileClass;
};
