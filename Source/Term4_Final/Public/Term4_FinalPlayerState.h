// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Spell.h"
#include "Term4_FinalPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TERM4_FINAL_API ATerm4_FinalPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

    ATerm4_FinalPlayerState();

    UPROPERTY(ReplicatedUsing = OnRep_UsedSpells, BlueprintReadOnly, Category = "Spells")
    TArray<TSubclassOf<USpell>> UsedSpells;


    UFUNCTION(BlueprintCallable, Category = "Spells")
    void Server_UseSpell(TSubclassOf<USpell> SpellClass);


    UFUNCTION(BlueprintPure, Category = "Spells")
    bool HasUsedSpell(TSubclassOf<USpell> SpellClass) const;

protected:
    UFUNCTION()
    void OnRep_UsedSpells();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
