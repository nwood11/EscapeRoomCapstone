#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "ManipObjectRotaryBase.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnRotaryValueChanged,
	float,
	CurrentValue,
	float,
	Percentage
);

UCLASS(Abstract)
class ESCAPEROOMCAPSTONE_API AManipObjectRotaryBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AManipObjectRotaryBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* RotaryMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAudioComponent* MovementAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText PromptText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotary")
	float CurrentTurnAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotary")
	float MinTurnAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotary")
	float MaxTurnAmount = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotary")
	float TurnSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotary")
	float RotationMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundBase* MovementSound = nullptr;

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
	UPROPERTY(BlueprintAssignable, Category="Rotary")
	FOnRotaryValueChanged OnValueChanged;

	UFUNCTION(BlueprintCallable, Category="Rotary")
	float GetTurnPercentage() const;

	UFUNCTION(BlueprintCallable, Category="Rotary")
	float GetCurrentTurnAmount() const;
	
	UFUNCTION(BlueprintCallable, Category="Rotary")
	virtual bool CanApplyDelta(float Delta) const;

	UFUNCTION(BlueprintCallable, Category="Rotary")
	void HandleManipulationDelta(float Delta);

	UFUNCTION(BlueprintCallable, Category="Rotary")
	void ApplyDelta(float Delta);

	UFUNCTION(BlueprintCallable, Category="Rotary")
	void BroadcastCurrentValue();

protected:
	virtual FRotator GetRotationForDelta(float AppliedDelta) const;

	virtual void OnDeltaApplied(float AppliedDelta);

	void UpdateMovementAudio(float AppliedDelta);
	void StopMovementAudio();

public:
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractDisplayName_Implementation() const override;
	virtual FText GetInteractPromptText_Implementation(AActor* Interactor) const override;
	virtual bool ShouldEnterManipulation_Implementation(AActor* Interactor) const override;
	virtual void OnBeginManipulation_Implementation(AActor* Interactor) override;
	virtual void OnEndManipulation_Implementation(AActor* Interactor) override;
};