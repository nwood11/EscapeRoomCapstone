// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "EscapeRoomCapstoneCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

/**
 * Delegate fired when the currently focused interactable changes.
 * This allows the HUD to be able to update without the character having direct references to UI components.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnInteractableFocusChanged,
	AActor*, FocusedActor,
	FText, DisplayName,
	FText, PromptText
);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AEscapeRoomCapstoneCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	// Currently looked-at interactable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess="true"))
	TObjectPtr<AActor> FocusedInteractable = nullptr;

	// Cache last UI values so HUD can refresh even when focus doesn't change.
	FText LastDisplayName;
	FText LastPromptText;
	bool bLastCanInteract = false;
	
	// Updates FocusedInteractable (and HUD) if it changed
	void UpdateInteractableFocus();

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;
	
public:
	AEscapeRoomCapstoneCharacter();

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	/** How far the player can interact with objects (trace distance) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionDistance = 250.f;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();
	
	// Manipulation Interaction
	
	// True when player is currently manipulating an object
	UPROPERTY()
	bool bIsManipulating = false;

	// The actor currently being manipulated
	UPROPERTY()
	AActor* ActiveManipulatedActor = nullptr;

	// Sensitivity multiplier
	UPROPERTY(EditAnywhere, Category="Interaction|Manipulation")
	float ManipulationSensitivity = 1.0f;

	// Enter manipulation mode
	void BeginManipulation(AActor* Target);

	// Exit manipulation mode
	void EndManipulation();
	
public:
	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	// UI can bind to this to update prompts when focus changes.
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractableFocusChanged OnInteractableFocusChanged;
};

