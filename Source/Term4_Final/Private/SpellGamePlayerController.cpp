#include "SpellGamePlayerController.h"
#include "SpellGameCharacter.h"
#include "SpellGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void ASpellGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalPlayerController())
    {
        if (HUDClass)
        {
            HUD = CreateWidget<USpellGameHUD>(this, HUDClass);
            if (HUD)
            {
                HUD->AddToPlayerScreen(0);
                HUD->BP_UpdateHealth(1.f);
                HUD->BP_UpdateKills(0);
                HUD->BP_UpdateDeaths(0);
                HUD->BP_UpdateScore(0);
            }
        }
    }
}

void ASpellGamePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    InPawn->OnDestroyed.AddDynamic(this, &ASpellGamePlayerController::OnPawnDestroyed);

    if (ASpellGameCharacter* SpellCharacter = Cast<ASpellGameCharacter>(InPawn))
    {
        // reset health bar to full on each new possession
        if (HUD)
        {
            HUD->BP_UpdateHealth(1.f);
        }
    }
}

void ASpellGamePlayerController::OnPawnHealthChanged(float HealthPercent)
{
    if (HUD)
    {
        HUD->BP_UpdateHealth(HealthPercent);
    }
}

void ASpellGamePlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
    DeathCount++;

    if (HUD)
    {
        HUD->BP_UpdateDeaths(DeathCount);
    }

    // credit the kill to whoever killed this character
    if (ASpellGameCharacter* DeadChar = Cast<ASpellGameCharacter>(DestroyedActor))
    {
        if (ASpellGamePlayerController* KillerPC = Cast<ASpellGamePlayerController>(DeadChar->GetLastInstigator()))
        {
            KillerPC->NotifyKill();
        }
    }

    // respawn
    TArray<AActor*> ActorList;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);

    if (ActorList.Num() > 0)
    {
        AActor* RandomPlayerStart = ActorList[FMath::RandRange(0, ActorList.Num() - 1)];
        const FTransform SpawnTransform = RandomPlayerStart->GetActorTransform();

        if (ASpellGameCharacter* RespawnedCharacter = GetWorld()->SpawnActor<ASpellGameCharacter>(CharacterClass, SpawnTransform))
        {
            Possess(RespawnedCharacter);
        }
    }
}

void ASpellGamePlayerController::NotifyKill()
{
    KillCount++;

    if (HUD)
    {
        HUD->BP_UpdateKills(KillCount);
        HUD->BP_UpdateScore(KillCount);
    }
}