// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SpellGameCharacter.h"

void ASpellGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}


void ASpellGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// subscribe to the pawn's OnDestroyed delegate
	InPawn->OnDestroyed.AddDynamic(this, &ASpellGamePlayerController::OnPawnDestroyed);

	// is this a shooter character?
	if (ASpellGameCharacter* SpellCharacter = Cast<ASpellGameCharacter>(InPawn))
	{
		
	}
}

void ASpellGamePlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{

	// find the player start
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		// select a random player start
		AActor* RandomPlayerStart = ActorList[FMath::RandRange(0, ActorList.Num() - 1)];

		// spawn a character at the player start
		const FTransform SpawnTransform = RandomPlayerStart->GetActorTransform();

		if (ASpellGameCharacter* RespawnedCharacter = GetWorld()->SpawnActor<ASpellGameCharacter>(CharacterClass, SpawnTransform))
		{
			// possess the character
			Possess(RespawnedCharacter);
		}
	}
}


