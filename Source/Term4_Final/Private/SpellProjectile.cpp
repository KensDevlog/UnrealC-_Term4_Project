// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellProjectile.h"

#include "SpellGameCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASpellProjectile::ASpellProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	Damage = 10.f;
	Speed = 3000.f;

	// Collission
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionComponent->InitSphereRadius(32.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(CollisionComponent);
	
	// Projectile Movement Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;
	
	SetLifeSpan(10.f);
}

// Called when the game starts or when spawned
void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpellProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (ASpellGameCharacter* HitCharacter = Cast<ASpellGameCharacter>(Other))
	{
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = Hit;
		DamageEvent.ShotDirection = GetVelocity().GetSafeNormal();
		DamageEvent.DamageTypeClass = UDamageType::StaticClass();
		
		HitCharacter->TakeDamage(Damage, DamageEvent, InstigatorController, this);
	}
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		HitEffect,
		HitLocation,
		HitNormal.Rotation()
	);

	Destroy();
}

