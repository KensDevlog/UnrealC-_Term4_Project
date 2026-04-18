// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpellGameCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpellGameMovementComponent;

UCLASS()
class TERM4_FINAL_API ASpellGameCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
	USpellGameMovementComponent* SpellGameMovementComponent;

public:
	// Sets default values for this character's properties
	ASpellGameCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<USpellGameMovementComponent> MovementComponent;
	
	UFUNCTION(Client, Reliable)
	void OnPossessed_Client(AController* NewController);
	
	// Inputs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	const TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	const TObjectPtr<UInputAction> MovementInputAction;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpInputAction;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintInputAction;
	
	
	
protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void OnMovementInputReceived(const FInputActionValue& Value);
	void OnLookInputReceived(const FInputActionValue& Value);
	void OnJumpInputReceived(const FInputActionValue& Value);
	void OnSprintInputReceived(const FInputActionValue& Value);
};
