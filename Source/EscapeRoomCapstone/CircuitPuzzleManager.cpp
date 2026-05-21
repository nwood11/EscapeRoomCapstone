#include "CircuitPuzzleManager.h"
#include "Engine/World.h"

ACircuitPuzzleManager::ACircuitPuzzleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACircuitPuzzleManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnCircuitBoard();
	RecalculatePowerFlow();
}

void ACircuitPuzzleManager::SpawnCircuitBoard()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	SpawnedNodes.Empty();
	NodeGrid.Empty();
	SourceNode = nullptr;
	OutputNodes.Empty();
	bPuzzleSolved = false;

	for (const FCircuitNodeConfig& Config : NodeConfigs)
	{
		const FIntPoint GridPosition(Config.Row, Config.Column);

		if (NodeGrid.Contains(GridPosition))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("CircuitPuzzleManager: Duplicate node position at Row %d, Column %d."),
				Config.Row,
				Config.Column
			);
			continue;
		}

		TSubclassOf<ACircuitNode> ClassToSpawn = GetNodeClassForType(Config.NodeType);

		if (!ClassToSpawn)
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("CircuitPuzzleManager: Missing node class for node at Row %d, Column %d."),
				Config.Row,
				Config.Column
			);
			continue;
		}

		const FVector LocalOffset =
			BoardOriginOffset
			+ FVector(
				0.f,
				Config.Column * TileSpacing,
				-Config.Row * TileSpacing
			);

		const FVector SpawnLocation = GetActorTransform().TransformPosition(LocalOffset);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		ACircuitNode* NewNode = World->SpawnActor<ACircuitNode>(
			ClassToSpawn,
			SpawnLocation,
			GetActorRotation(),
			SpawnParams
		);

		if (!NewNode)
		{
			continue;
		}

		const TArray<ECircuitDirection> BaseConnections =
			GetBaseConnectionsForType(Config.NodeType);

		NewNode->InitializeNode(
			this,
			Config.Row,
			Config.Column,
			Config.StartingRotationIndex,
			BaseConnections,
			Config.bIsSource,
			Config.bIsOutput
		);

		SpawnedNodes.Add(NewNode);
		NodeGrid.Add(GridPosition, NewNode);

		if (Config.bIsSource)
		{
			if (SourceNode)
			{
				UE_LOG(LogTemp, Warning, TEXT("CircuitPuzzleManager: Multiple source nodes found. Using latest."));
			}

			SourceNode = NewNode;
		}

		if (Config.bIsOutput)
		{
			OutputNodes.Add(NewNode);
		}
	}

	if (!SourceNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("CircuitPuzzleManager: No source node configured."));
	}

	if (OutputNodes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("CircuitPuzzleManager: No output node configured."));
	}
}

void ACircuitPuzzleManager::RecalculatePowerFlow()
{
	ClearPoweredStates();

	if (!SourceNode || OutputNodes.IsEmpty())
	{
		return;
	}

	PropagatePowerFromSource();

	bool bNowSolved = true;

	for (ACircuitNode* Output : OutputNodes)
	{
		if (!Output || !Output->IsPowered())
		{
			bNowSolved = false;
			break;
		}
	}

	if (bNowSolved && !bPuzzleSolved)
	{
		bPuzzleSolved = true;
		OnCircuitPuzzleSolved.Broadcast();
	}
	else if (!bNowSolved)
	{
		bPuzzleSolved = false;
	}
}

void ACircuitPuzzleManager::ClearPoweredStates()
{
	for (ACircuitNode* Node : SpawnedNodes)
	{
		if (Node)
		{
			Node->SetPowered(false);
		}
	}
}

void ACircuitPuzzleManager::PropagatePowerFromSource()
{
	struct FNeighborCheck
	{
		int32 RowOffset;
		int32 ColumnOffset;
		ECircuitDirection OutDirection;
		ECircuitDirection InDirection;
	};

	static const FNeighborCheck Checks[] =
	{
		{ -1,  0, ECircuitDirection::Up,    ECircuitDirection::Down },
		{  0,  1, ECircuitDirection::Right, ECircuitDirection::Left },
		{  1,  0, ECircuitDirection::Down,  ECircuitDirection::Up },
		{  0, -1, ECircuitDirection::Left,  ECircuitDirection::Right }
	};

	TQueue<ACircuitNode*> Queue;
	TSet<ACircuitNode*> Visited;

	SourceNode->SetPowered(true);
	Queue.Enqueue(SourceNode);
	Visited.Add(SourceNode);

	while (!Queue.IsEmpty())
	{
		ACircuitNode* CurrentNode = nullptr;
		Queue.Dequeue(CurrentNode);

		if (!CurrentNode)
		{
			continue;
		}

		for (const FNeighborCheck& Check : Checks)
		{
			ACircuitNode* Neighbor = GetNodeAt(
				CurrentNode->GetRow() + Check.RowOffset,
				CurrentNode->GetColumn() + Check.ColumnOffset
			);

			if (!Neighbor || Visited.Contains(Neighbor))
			{
				continue;
			}

			const bool bCanFlow =
				CurrentNode->HasConnection(Check.OutDirection)
				&& Neighbor->HasConnection(Check.InDirection);

			if (bCanFlow)
			{
				Neighbor->SetPowered(true);
				Visited.Add(Neighbor);
				Queue.Enqueue(Neighbor);
			}
		}
	}
}

ACircuitNode* ACircuitPuzzleManager::GetNodeAt(int32 Row, int32 Column) const
{
	ACircuitNode* const* FoundNode = NodeGrid.Find(FIntPoint(Row, Column));
	return FoundNode ? *FoundNode : nullptr;
}

TArray<ECircuitDirection> ACircuitPuzzleManager::GetBaseConnectionsForType(ECircuitNodeType NodeType) const
{
	switch (NodeType)
	{
	case ECircuitNodeType::Straight:
		return {
			ECircuitDirection::Up,
			ECircuitDirection::Down
		};

	case ECircuitNodeType::Corner:
		return {
			ECircuitDirection::Up,
			ECircuitDirection::Right
		};

	case ECircuitNodeType::Tee:
		return {
			ECircuitDirection::Up,
			ECircuitDirection::Left,
			ECircuitDirection::Right
		};

	case ECircuitNodeType::Cross:
		return {
			ECircuitDirection::Up,
			ECircuitDirection::Right,
			ECircuitDirection::Down,
			ECircuitDirection::Left
		};
		
	case ECircuitNodeType::DeadEnd:
		return {
			ECircuitDirection::Right
		};
		
	default:
		return {};
	}
}

TSubclassOf<ACircuitNode> ACircuitPuzzleManager::GetNodeClassForType(ECircuitNodeType NodeType) const
{
	switch (NodeType)
	{
	case ECircuitNodeType::DeadEnd:
		return DeadEndNodeClass;

	case ECircuitNodeType::Straight:
		return StraightNodeClass;

	case ECircuitNodeType::Corner:
		return CornerNodeClass;

	case ECircuitNodeType::Tee:
		return TeeNodeClass;

	case ECircuitNodeType::Cross:
		return CrossNodeClass;

	default:
		return nullptr;
	}
}