#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValveGauge.generated.h"

class AManipObjectValve;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGaugeTargetMetChanged, bool, bTargetMet);

USTRUCT(BlueprintType)
struct FValveGaugeInfluence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	AManipObjectValve* Valve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float Influence = 1.0f;
};

UCLASS()
class ESCAPEROOMCAPSTONE_API AValveGauge : public AActor
{
	GENERATED_BODY()

public:
	AValveGauge();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* GaugeBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* GaugeIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* TargetZoneIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gauge")
	TArray<FValveGaugeInfluence> ValveInfluences;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gauge")
	float IndicatorMinX = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gauge")
	float IndicatorMaxX = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gauge")
	float TargetValue = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gauge")
	float TargetTolerance = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gauge")
	float CurrentValue = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gauge")
	bool bTargetMet = false;

public:
	UPROPERTY(BlueprintAssignable, Category="Gauge")
	FOnGaugeTargetMetChanged OnGaugeTargetMetChanged;

	UFUNCTION()
	void OnSourceValveChanged(float NewValue, float Percentage);

	UFUNCTION(BlueprintCallable, Category="Gauge")
	bool IsTargetMet() const;

protected:
	void RecalculateGauge();
	void SetTargetMet(bool bNewTargetMet);
	void UpdateTargetZoneIndicator();
};