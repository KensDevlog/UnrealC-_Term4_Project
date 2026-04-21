#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpellGameHUD.h"
#include "SpellGamePlayerController.generated.h"

class ASpellGameCharacter;

UCLASS()
class TERM4_FINAL_API ASpellGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Respawn")
	TSubclassOf<ASpellGameCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<USpellGameHUD> HUDClass;

	TObjectPtr<USpellGameHUD> HUD;

	int32 KillCount = 0;
	int32 DeathCount = 0;

public:
	void NotifyKill();

	// public so SpellGameCharacter can bind to it from OnPossessed_Client
	UFUNCTION()
	void OnPawnHealthChanged(float HealthPercent);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);
};