// Copyright Epic Games, Inc. All Rights Reserved.

#include "EscapeRoomCapstoneCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EscapeRoomCapstone.h"
#include "InteractableInterface.h"

AEscapeRoomCapstoneCharacter::AEscapeRoomCapstoneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	
	// Enable tick
	PrimaryActorTick.bCanEverTick = true;
}

void AEscapeRoomCapstoneCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateInteractableFocus();
}

/**
 * Performs a forward line trace each frame to detect interactable objects.
 *
 * Responsibilities:
 * - Determine which actor the player is currently looking at
 * - Check if the actor implements InteractableInterface
 * - Pull metadata (display name, prompt text, interactability state)
 * - Broadcast delegate event when the focused interactable changes
 * The HUD listens to delegate events instead of being updated directly.
 */
void AEscapeRoomCapstoneCharacter::UpdateInteractableFocus()
{
	if (!FirstPersonCameraComponent) return;

	// Start and end points for the line trace
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * InteractionDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	AActor* NewFocused = nullptr;
	
	// Check if we hit an actor
	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			// If the hit actor has the interactable interface, set it as a potential new focused actor.
			NewFocused = HitActor;
		}
	}

	const bool bFocusChanged = (NewFocused != FocusedInteractable);
	FocusedInteractable = NewFocused;

	// Lost focus
	if (!FocusedInteractable)
	{
		// reset cache so next focus starts clean
		LastDisplayName = FText::GetEmpty();
		LastPromptText = FText::GetEmpty();
		bLastCanInteract = false;

		OnInteractableFocusChanged.Broadcast(nullptr, LastDisplayName, LastPromptText, bLastCanInteract);
		return;
	}

	// Pull metadata via interface
	const bool bCan = IInteractableInterface::Execute_CanInteract(FocusedInteractable, this);
	const FText DisplayName = IInteractableInterface::Execute_GetInteractDisplayName(FocusedInteractable);
	const FText PromptText = IInteractableInterface::Execute_GetInteractPromptText(FocusedInteractable, this);

	// Only check if any of the metadata (e.g. prompt text) has changed
	const bool bUiChanged =
		bFocusChanged ||
		!DisplayName.EqualTo(LastDisplayName) ||
		!PromptText.EqualTo(LastPromptText) ||
		(bCan != bLastCanInteract);

	if (bUiChanged)
	{
		LastDisplayName = DisplayName;
		LastPromptText = PromptText;
		bLastCanInteract = bCan;
		// Update HUD prompt widget
		OnInteractableFocusChanged.Broadcast(FocusedInteractable, DisplayName, PromptText, bCan);
	}
}

void AEscapeRoomCapstoneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEscapeRoomCapstoneCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEscapeRoomCapstoneCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEscapeRoomCapstoneCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEscapeRoomCapstoneCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AEscapeRoomCapstoneCharacter::LookInput);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEscapeRoomCapstoneCharacter::TryInteract);
	}
	else
	{
		UE_LOG(LogEscapeRoomCapstone, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEscapeRoomCapstoneCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AEscapeRoomCapstoneCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AEscapeRoomCapstoneCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AEscapeRoomCapstoneCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AEscapeRoomCapstoneCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AEscapeRoomCapstoneCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}


/**
 * Called when player presses the interact input.
 *
 * Uses the currently focused interactable determined by the trace system.
 * Checks CanInteract() before executing OnInteract().
 */
void AEscapeRoomCapstoneCharacter::TryInteract()
{
	// Check if we have a focused object to interact with
	if (!FocusedInteractable) return;

	// Check if the focused object has the interactable interface
	if (FocusedInteractable->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		// Check if the object is set to be interactable
		const bool bCan = IInteractableInterface::Execute_CanInteract(FocusedInteractable, this);
		if (bCan)
		{
			// Interact with the object
			IInteractableInterface::Execute_OnInteract(FocusedInteractable, this);
		}
	}
}