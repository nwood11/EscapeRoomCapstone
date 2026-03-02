// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

/**
 * InteractableInterface
 *
 * Defines the contract for any actor that can be interacted with by the player.
 *
 * This allows the player character to detect and interact with objects without
 * needing to know their concrete class types (doors, puzzles, pickups, etc.).
 *
 * The interface exposes:
 * - OnInteract(): executes the interaction logic
 * - Metadata getters for HUD prompts and UI
 * - CanInteract(): allows objects to gate interaction based on game state
 *
 * Used by:
 * - Player character line trace system
 * - HUD prompt display logic
 */

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class ESCAPEROOMCAPSTONE_API IInteractableInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called when an actor attempts to interact with this object.
	 * Implement this on interactable actors (doors, pickups, puzzle components, etc.).
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void OnInteract(AActor* Interactor);

	// Returns a short name for the interactable (ex: "Door", "Keycard", "Fuse Box").
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	FText GetInteractDisplayName() const;

	// Returns the prompt shown to the player (ex: "Press E to open").
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	FText GetInteractPromptText(AActor* Interactor) const;
	
	// If true, interacting should enter manipulation mode instead of doing a one-shot OnInteract.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction|Manipulation")
	bool ShouldEnterManipulation(AActor* Interactor) const;
	
	// Called once when manipulation mode begins (after the character locks input).
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction|Manipulation")
	void OnBeginManipulation(AActor* Interactor);

	// Called every time mouse input is received during manipulation to pass axis deltas
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction|Manipulation")
	void OnManipulateAxis(AActor* Interactor, float XAxisDelta, float YAxisDelta);

	// Called once when manipulation mode ends
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction|Manipulation")
	void OnEndManipulation(AActor* Interactor);
	
};
