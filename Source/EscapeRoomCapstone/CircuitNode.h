#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "CircuitNode.generated.h"

class ACircuitPuzzleManager;

UENUM(BlueprintType)
enum class ECircuitDirection : uint8
{
	Up = 0		UMETA(DisplayName = "Up"),
	Right = 1	UMETA(DisplayName = "Right"),
	Down = 2	UMETA(DisplayName = "Down"),
	Left = 3	UMETA(DisplayName = "Left")
};

UENUM(BlueprintType)
enum class ECircuitNodeType : uint8
{
	DeadEnd  UMETA(DisplayName = "Dead End"),
	Straight UMETA(DisplayName = "Straight"),
	Corner   UMETA(DisplayName = "Corner"),
	Tee      UMETA(DisplayName = "Tee"),
	Cross    UMETA(DisplayName = "Cross")
};

UCLASS()
class ESCAPEROOMCAPSTONE_API ACircuitNode
	: public AActor
	, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ACircuitNode();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Circuit")
	UStaticMeshComponent* NodeMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	ACircuitPuzzleManager* PuzzleManager = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	int32 Row = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	int32 Column = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	int32 RotationIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	bool bPowered = false;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	TArray<ECircuitDirection> BaseConnections;
	
public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	bool bIsSource = false;

	UPROPERTY(BlueprintReadOnly, Category = "Circuit")
	bool bIsOutput = false;
	
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void InitializeNode(
		ACircuitPuzzleManager* InPuzzleManager,
		int32 InRow,
		int32 InColumn,
		int32 InStartingRotationIndex,
		const TArray<ECircuitDirection>& InBaseConnections,
		bool bInIsSource,
		bool bInIsOutput
	);

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void RotateNode();

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	bool HasConnection(ECircuitDirection Direction) const;

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void SetPowered(bool bNewPowered);

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	bool IsPowered() const { return bPowered; }

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	int32 GetRow() const { return Row; }

	UFUNCTION(BlueprintCallable, Category = "Circuit")
	int32 GetColumn() const { return Column; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Circuit")
	void BP_UpdatePowerVisuals(bool bNewPowered);

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	virtual FText GetInteractDisplayName_Implementation() const override;

	virtual FText GetInteractPromptText_Implementation(AActor* Interactor) const override;
};