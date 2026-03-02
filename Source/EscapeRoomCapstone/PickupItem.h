#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "PickupItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class ESCAPEROOMCAPSTONE_API APickupItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	APickupItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* Mesh;

	// Inventory ID used by GameInstance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FName ItemID;

	// Quantity granted on pickup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	int32 Quantity = 1;

	// Display name shown in HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText DisplayName;

	// Prompt text shown in HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText PromptText = FText::FromString("Press E to pick up");

public:

	// Interactable Interface implementations

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	virtual FText GetInteractDisplayName_Implementation() const override;

	virtual FText GetInteractPromptText_Implementation(AActor* Interactor) const override;

};
