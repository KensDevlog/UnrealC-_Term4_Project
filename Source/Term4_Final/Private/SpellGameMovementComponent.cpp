// Ken Arigo @ VFS 2026


#include "Public/SpellGameMovementComponent.h"

#include "GameFramework/Character.h"


bool USpellGameMovementComponent::FSavedMove_SpellGameCharacter::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                                ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_SpellGameCharacter* NewSpellGameMove = static_cast<FSavedMove_SpellGameCharacter*>(NewMove.Get());
	
	if (Saved_bWantsToSprint != NewSpellGameMove->Saved_bWantsToSprint) return false;
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USpellGameMovementComponent::FSavedMove_SpellGameCharacter::Clear()
{
	FSavedMove_Character::Clear();
	
	Saved_bWantsToSprint = 0;
}

uint8 USpellGameMovementComponent::FSavedMove_SpellGameCharacter::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();
	
	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	
	return Result;
}

void USpellGameMovementComponent::FSavedMove_SpellGameCharacter::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	
	USpellGameMovementComponent* CharacterMovement = Cast<USpellGameMovementComponent>(C->GetCharacterMovement());
	
	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void USpellGameMovementComponent::FSavedMove_SpellGameCharacter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	
	USpellGameMovementComponent* CharacterMovement = Cast<USpellGameMovementComponent>(C->GetCharacterMovement());
	
	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

USpellGameMovementComponent::FNetworkPredictionData_Client_SpellGame::FNetworkPredictionData_Client_SpellGame(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr USpellGameMovementComponent::FNetworkPredictionData_Client_SpellGame::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_SpellGameCharacter());
}

FNetworkPredictionData_Client* USpellGameMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);
	
	if (ClientPredictionData == nullptr)
	{
		USpellGameMovementComponent* MutableThis = const_cast<USpellGameMovementComponent*>(this);
		
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_SpellGame(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	
	return ClientPredictionData;
}

// Sets default values for this component's properties
USpellGameMovementComponent::USpellGameMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USpellGameMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}


// Called when the game starts
void USpellGameMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpellGameMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpellGameMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	
	if (MovementMode == MOVE_Walking)
	{
		Safe_bWantsToSprint ? MaxWalkSpeed = Sprint_MaxWalkSpeed : MaxWalkSpeed = Walk_MaxWalkSpeed;
	}
}

void USpellGameMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void USpellGameMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

