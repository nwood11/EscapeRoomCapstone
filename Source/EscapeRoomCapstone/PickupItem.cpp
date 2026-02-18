#include "PickupItem.h"
#include "Components/StaticMeshComponent.h"
#include "EscapeGameInstance.h"
#include "Kismet/GameplayStatics.h"

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	Mesh->SetSimulatePhysics(false);
}

void APickupItem::OnInteract_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	UEscapeGameInstance* GI = Cast<UEscapeGameInstance>(
		UGameplayStatics::GetGameInstance(this)
	);

	if (!GI) return;

	GI->AddItem(ItemID, Quantity);

	// Destroy pickup after collection
	Destroy();
}

FText APickupItem::GetInteractDisplayName_Implementation() const
{
	return DisplayName;
}

FText APickupItem::GetInteractPromptText_Implementation(AActor* Interactor) const
{
	return PromptText;
}

bool APickupItem::CanInteract_Implementation(AActor* Interactor) const
{
	return !ItemID.IsNone() && Quantity > 0;
}
