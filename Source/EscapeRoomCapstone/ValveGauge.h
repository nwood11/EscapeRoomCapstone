#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValveGauge.generated.h"

class UStaticMeshComponent;
class AManipObjectValve;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGaugeTargetMetChanged, bool, bIsTargetMet);

UCLASS()
class ESCAPEROOMCAPSTONE_API AValveGauge : public AActor
{
	GENERATED_BODY()

public:
	AValveGauge();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* GaugeBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* GaugeIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	AManipObjectValve* ConnectedValve = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* TargetZoneIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float TargetZoneThickness = 6.0f;

	UFUNCTION(BlueprintCallable, Category="Gauge")
	void UpdateTargetZoneIndicator();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float CurrentValue = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float TargetValue = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float TargetTolerance = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float IndicatorMinX = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gauge")
	float IndicatorMaxX = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gauge")
	bool bTargetMet = false;

	UPROPERTY(BlueprintAssignable, Category="Gauge")
	FOnGaugeTargetMetChanged OnGaugeTargetMetChanged;

public:
	UFUNCTION(BlueprintCallable, Category="Gauge")
	void UpdateGauge(float NewValue, float Percentage);

	UFUNCTION(BlueprintCallable, Category="Gauge")
	bool IsTargetMet() const;

private:
	void SetTargetMet(bool bNewTargetMet);
};