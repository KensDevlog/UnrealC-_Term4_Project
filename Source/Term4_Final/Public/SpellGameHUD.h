#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellGameHUD.generated.h"

UCLASS(Abstract)
class TERM4_FINAL_API USpellGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void BP_UpdateHealth(float HealthPercent);

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void BP_UpdateKills(int32 Kills);

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void BP_UpdateDeaths(int32 Deaths);

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void BP_UpdateScore(int32 Score);
};