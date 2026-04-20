// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpellGamePlayerController.generated.h"


class ASpellGameCharacter;

/**
 * 
 */
UCLASS()
class TERM4_FINAL_API ASpellGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Character class to respawn when the possessed pawn is destroyed */
	UPROPERTY(EditAnywhere, Category = "Shooter|Respawn")
	TSubclassOf<ASpellGameCharacter> CharacterClass;

	/** Gameplay Initialization */
	virtual void BeginPlay() override;

	/** Pawn initialization */
	virtual void OnPossess(APawn* InPawn) override;

	/** Called if the possessed pawn is destroyed */
	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);

};
