// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/SpellGameCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Public/SpellGameMovementComponent.h"

ASpellGameCharacter::ASpellGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USpellGameMovementComponent>(CharacterMovementComponentName))
{
	SpellGameMovementComponent = Cast<USpellGameMovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void ASpellGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ASpellGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	OnPossessed_Client(NewController);
}

// Called every frame
void ASpellGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpellGameCharacter::OnPossessed_Client_Implementation(AController* NewController)
{
	if (APlayerController* NewPlayerController = Cast<APlayerController>(NewController))
	{
		if (ULocalPlayer* LocalPlayer = NewPlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called to bind functionality to input
void ASpellGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MovementInputAction)
		{
			EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ASpellGameCharacter::OnMovementInputReceived);
		}

		if (LookInputAction)
		{
			EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASpellGameCharacter::OnLookInputReceived);
		}

		if (JumpInputAction)
		{
			EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASpellGameCharacter::OnJumpInputReceived);
		}

		if (SprintInputAction)
		{
			EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ASpellGameCharacter::OnSprintInputReceived);
			EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ASpellGameCharacter::OnSprintInputReceived);
		}
	}
}


void ASpellGameCharacter::OnMovementInputReceived(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FVector ForwardVector = GetActorForwardVector();
		const FVector RightVector = GetActorRightVector();
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void ASpellGameCharacter::OnLookInputReceived(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASpellGameCharacter::OnJumpInputReceived(const FInputActionValue& Value)
{
	Jump();
}

void ASpellGameCharacter::OnSprintInputReceived(const FInputActionValue& Value)
{
	(Value.Get<bool>()) ? SpellGameMovementComponent->SprintPressed() : SpellGameMovementComponent->SprintReleased();
}

