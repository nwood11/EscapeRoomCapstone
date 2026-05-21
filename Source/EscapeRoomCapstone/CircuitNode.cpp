#include "CircuitNode.h"
#include "CircuitPuzzleManager.h"
#include "Components/StaticMeshComponent.h"

ACircuitNode::ACircuitNode()
{
	PrimaryActorTick.bCanEverTick = false;

	NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
	SetRootComponent(NodeMesh);
}

void ACircuitNode::InitializeNode(
	ACircuitPuzzleManager* InPuzzleManager,
	int32 InRow,
	int32 InColumn,
	int32 InStartingRotationIndex,
	const TArray<ECircuitDirection>& InBaseConnections,
	bool bInIsSource,
	bool bInIsOutput
)
{
	PuzzleManager = InPuzzleManager;
	Row = InRow;
	Column = InColumn;
	bIsSource = bInIsSource;
	bIsOutput = bInIsOutput;
	RotationIndex = ((InStartingRotationIndex % 4) + 4) % 4;

	BaseConnections = InBaseConnections;

	const FRotator BoardRotation = PuzzleManager
		? PuzzleManager->GetActorRotation()
		: FRotator::ZeroRotator;
	
	SetActorRotation(
		BoardRotation + FRotator(0.f, 0.f, RotationIndex * 90.f)
	);
}

void ACircuitNode::RotateNode()
{
	RotationIndex = (RotationIndex + 1) % 4;

	const FRotator BoardRotation = PuzzleManager
		? PuzzleManager->GetActorRotation()
		: FRotator::ZeroRotator;

	SetActorRotation(
		BoardRotation + FRotator(0.f, 0.f, RotationIndex * 90.f)
	);

	if (PuzzleManager)
	{
		PuzzleManager->RecalculatePowerFlow();
	}
}

bool ACircuitNode::HasConnection(ECircuitDirection Direction) const
{
	const int32 TargetDirection = static_cast<int32>(Direction);

	for (ECircuitDirection BaseDirection : BaseConnections)
	{
		const int32 BaseValue = static_cast<int32>(BaseDirection);

		const int32 RotatedValue =
			(BaseValue + RotationIndex) % 4;

		if (RotatedValue == TargetDirection)
		{
			return true;
		}
	}

	return false;
}

void ACircuitNode::SetPowered(bool bNewPowered)
{
	bPowered = bNewPowered;

	BP_UpdatePowerVisuals(bPowered);
}

void ACircuitNode::OnInteract_Implementation(AActor* Interactor)
{
	RotateNode();
}

FText ACircuitNode::GetInteractDisplayName_Implementation() const
{
	return FText::FromString(TEXT("Circuit Node"));
}

FText ACircuitNode::GetInteractPromptText_Implementation(AActor* Interactor) const
{
	return FText::FromString(TEXT("Press F to rotate"));
}