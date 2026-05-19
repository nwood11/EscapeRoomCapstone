#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircuitNode.h"
#include "CircuitPuzzleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCircuitPuzzleSolved);

USTRUCT(BlueprintType)
struct FCircuitNodeConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circuit")
	int32 Row = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circuit")
	int32 Column = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circuit")
	int32 StartingRotationIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circuit")
	ECircuitNodeType NodeType = ECircuitNodeType::Straight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circuit")
	bool bIsSource = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Circuit")
	bool bIsOutput = false;
};

UCLASS()
class ESCAPEROOMCAPSTONE_API ACircuitPuzzleManager : public AActor
{
	GENERATED_BODY()

public:
	ACircuitPuzzleManager();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Node Classes")
	TSubclassOf<ACircuitNode> DeadEndNodeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Node Classes")
	TSubclassOf<ACircuitNode> StraightNodeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Node Classes")
	TSubclassOf<ACircuitNode> CornerNodeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Node Classes")
	TSubclassOf<ACircuitNode> TeeNodeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Node Classes")
	TSubclassOf<ACircuitNode> CrossNodeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Setup")
	TArray<FCircuitNodeConfig> NodeConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Setup")
	float TileSpacing = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circuit|Setup")
	FVector BoardOriginOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit|Runtime")
	TArray<ACircuitNode*> SpawnedNodes;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit|Runtime")
	ACircuitNode* SourceNode = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit|Runtime")
	ACircuitNode* OutputNode = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit|Runtime")
	bool bPuzzleSolved = false;

public:
	UPROPERTY(BlueprintAssignable, Category = "Circuit")
	FOnCircuitPuzzleSolved OnCircuitPuzzleSolved;

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void SpawnCircuitBoard();

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void RecalculatePowerFlow();

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	bool IsPuzzleSolved() const { return bPuzzleSolved; }

private:
	TMap<FIntPoint, ACircuitNode*> NodeGrid;

	void ClearPoweredStates();
	void PropagatePowerFromSource();

	ACircuitNode* GetNodeAt(int32 Row, int32 Column) const;
	
	TArray<ECircuitDirection> GetBaseConnectionsForType(ECircuitNodeType NodeType) const;
	
	TSubclassOf<ACircuitNode> GetNodeClassForType(ECircuitNodeType NodeType) const;
	
};