#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "ManipObjectLever.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeverValueChanged, float, CurrentValue, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeverLimitReached);

UCLASS()
class ESCAPEROOMCAPSTONE_API AManipObjectLever : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AManipObjectLever();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* LeverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact")
	FText PromptText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Lever")
	float CurrentTurnAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Lever")
	float MinTurnAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Lever")
	float MaxTurnAmount = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Lever")
	float TurnSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Lever")
	float RotationMultiplier = 1.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	UAudioComponent* LeverMovingAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundBase* LeverMovingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	float MinMoveVolume = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	float MaxMoveVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	float MinMovePitch = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	float MaxMovePitch = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	float FullSpeedDelta = 2.0f;

public:
	UPROPERTY(BlueprintAssignable, Category="Lever")
	FOnLeverValueChanged OnValueChanged;

	UPROPERTY(BlueprintAssignable, Category="Lever")
	FOnLeverLimitReached OnMinReached;

	UPROPERTY(BlueprintAssignable, Category="Lever")
	FOnLeverLimitReached OnMaxReached;

	float GetTurnPercentage() const;

	void BroadcastCurrentValue();

	bool CanApplyDelta(float Delta) const;

	void ApplyLeverDelta(float Delta);

	void UpdateLeverAudio(float AppliedDelta);
	void StopLeverAudio();
	
	// InteractableInterface
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractDisplayName_Implementation() const override;
	virtual FText GetInteractPromptText_Implementation(AActor* Interactor) const override;
	virtual bool ShouldEnterManipulation_Implementation(AActor* Interactor) const override;
	virtual void OnBeginManipulation_Implementation(AActor* Interactor) override;
	virtual void OnManipulateAxis_Implementation(AActor* Interactor, float XAxisDelta, float YAxisDelta) override;
	virtual void OnEndManipulation_Implementation(AActor* Interactor) override;
};