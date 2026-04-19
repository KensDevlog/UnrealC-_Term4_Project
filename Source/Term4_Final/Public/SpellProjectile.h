// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellProjectile.generated.h"

class UCameraComponent;
class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable, BlueprintType)
class TERM4_FINAL_API ASpellProjectile : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * 
	 */
	ASpellProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(
		UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spell")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spell")
	TObjectPtr<USphereComponent> CollisionComponent;
	
	// Constant Visual
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	TObjectPtr<UNiagaraSystem> SpellEffect;
	
	// Spawns on Impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	TObjectPtr<UNiagaraSystem> HitEffect;
	
	UPROPERTY()
	TObjectPtr<AController> InstigatorController;
	
	float Damage;
	float Speed;
	
};
