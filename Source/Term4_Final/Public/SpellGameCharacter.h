#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpellGameCharacter.generated.h"

class UCameraComponent;
class UWandComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpellGameMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, ASpellGameCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, HealthPercent);

UCLASS()
class TERM4_FINAL_API ASpellGameCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
    USpellGameMovementComponent* SpellGameMovementComponent;

public:
    ASpellGameCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(Client, Reliable, BlueprintCallable)
    void OnPossessed_Client(AController* NewController);

    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnCharacterDied OnCharacterDied;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath(ASpellGameCharacter* Killer);

	/** Called to allow Blueprint code to react to this character's death */
	UFUNCTION(BlueprintImplementableEvent, Category = "Spell", meta = (DisplayName = "On Death"))
	void BP_OnDeath(ASpellGameCharacter* Killer);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnHealthChanged OnHealthChanged;

    // Inputs
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Input")
    const TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input")
    const TObjectPtr<UInputAction> MovementInputAction;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> LookInputAction;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> JumpInputAction;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> SprintInputAction;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> ShootInputAction;

    // Stats
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
    float MaxHealth = 100.f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
    float CurrentHealth = 0.f;

    // Wand Component
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Wand")
    TObjectPtr<UWandComponent> WandComponent;

    // First Person Mesh
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character")
    TObjectPtr<USkeletalMeshComponent> FirstPersonCharacterMesh;

    // Camera
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera")
    TObjectPtr<UCameraComponent> CameraComponent;

    UFUNCTION(Client, Reliable)
    void Client_UpdateHealth(float HealthPercent);

    AController* GetLastInstigator() const { return LastInstigator; }

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void OnMovementInputReceived(const FInputActionValue& Value);
    void OnLookInputReceived(const FInputActionValue& Value);
    void OnJumpInputReceived(const FInputActionValue& Value);
    void OnSprintInputReceived(const FInputActionValue& Value);
    void OnShootInputReceived(const FInputActionValue& Value);

private:
    UPROPERTY()
    TObjectPtr<AController> LastInstigator;
};