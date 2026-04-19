// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WandComponent.generated.h"


class UCameraComponent;
class USpell;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TERM4_FINAL_API UWandComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// Members
	UPROPERTY()
	TObjectPtr<USpell> EquippedSpell;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USpell> EquippedSpellClass;
	
	// Member functions
public:
	UFUNCTION(BlueprintCallable)
	void ShootSpell();
	
	UFUNCTION(BlueprintCallable)
	void HandleShootInput(bool InputDown);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	bool IsTryingToCast;
	
	UPROPERTY(BlueprintReadOnly)
	bool CanCast = true;
	
	// For non burst shot timings
	UPROPERTY(BlueprintReadOnly, Category="Shot Interval")
	float NextPrimaryIntervalShotTime = 0;
	
	// for burst shot timings
	UPROPERTY(BlueprintReadOnly, Category="Shot Interval")
	float NextSecondaryIntervalShotTime = 0;
	
	UPROPERTY(BlueprintReadOnly, Category="Shot Interval")
	int BurstShotIteration = 0;
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;
};
