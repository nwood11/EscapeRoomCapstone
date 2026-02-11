// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
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

	// Returns whether the object can currently be interacted with.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	bool CanInteract(AActor* Interactor) const;
	
	
};
