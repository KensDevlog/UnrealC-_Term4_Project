// Fill out your copyright notice in the Description page of Project Settings.


#include "Term4_FinalPlayerState.h"
#include "Net/UnrealNetwork.h"

void ATerm4_FinalPlayerState::Server_UseSpell(TSubclassOf<USpell> SpellClass)
{
	if (!HasAuthority() || !SpellClass) return;

	UsedSpells.AddUnique(SpellClass);
}

bool ATerm4_FinalPlayerState::HasUsedSpell(TSubclassOf<USpell> SpellClass) const
{
	return UsedSpells.Contains(SpellClass);
}

void ATerm4_FinalPlayerState::OnRep_UsedSpells()
{
}

void ATerm4_FinalPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATerm4_FinalPlayerState, UsedSpells);
}
