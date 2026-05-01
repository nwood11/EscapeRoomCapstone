#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "ManipObjectValve.generated.h"


class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValveValueChanged, float, NewValue, float, Percentage);

UCLASS()
class ESCAPEROOMCAPSTONE_API AManipObjectValve : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AManipObjectValve();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ValveMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve")
	float CurrentTurnAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve")
	float MinTurnAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve")
	float MaxTurnAmount = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve")
	float TurnSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve")
	float RotationMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve|Connection")
	AManipObjectValve* ConnectedValve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve|Connection")
	float ConnectedValveMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText DisplayName = FText::FromString(TEXT("Valve"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText PromptText = FText::FromString(TEXT("Press F to turn"));

	UPROPERTY(BlueprintAssignable, Category="Valve")
	FOnValveValueChanged OnValueChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAudioComponent* ValveTurningAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* ValveTurningSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	float MinTurnVolume = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	float MaxTurnVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	float MinTurnPitch = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	float MaxTurnPitch = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	float FullSpeedDelta = 3.0f;

public:
	UFUNCTION(BlueprintCallable, Category="Valve")
	float GetTurnPercentage() const;

	UFUNCTION(BlueprintCallable, Category="Valve")
	bool CanApplyDelta(float Delta) const;

	UFUNCTION(BlueprintCallable, Category="Valve")
	void ApplyValveDelta(float Delta);

	UFUNCTION(BlueprintCallable, Category="Valve")
	void BroadcastCurrentValue();

public:
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractDisplayName_Implementation() const override;
	virtual FText GetInteractPromptText_Implementation(AActor* Interactor) const override;
	virtual bool ShouldEnterManipulation_Implementation(AActor* Interactor) const override;
	virtual void OnBeginManipulation_Implementation(AActor* Interactor) override;
	virtual void OnManipulateAxis_Implementation(AActor* Interactor, float XAxisDelta, float YAxisDelta) override;
	virtual void OnEndManipulation_Implementation(AActor* Interactor) override;
	
private:
	void UpdateValveAudio(float AppliedDelta);
	void StopValveAudio();
	
};