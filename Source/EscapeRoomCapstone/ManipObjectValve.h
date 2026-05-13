#pragma once

#include "CoreMinimal.h"
#include "ManipObjectRotaryBase.h"
#include "ManipObjectValve.generated.h"

UCLASS()
class ESCAPEROOMCAPSTONE_API AManipObjectValve : public AManipObjectRotaryBase
{
	GENERATED_BODY()

public:
	AManipObjectValve();

protected:
	virtual void BeginPlay() override;
	virtual FRotator GetRotationForDelta(float AppliedDelta) const override;
	virtual void OnDeltaApplied(float AppliedDelta) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve|Connection")
	AManipObjectValve* ConnectedValve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valve|Connection")
	float ConnectedValveMultiplier = 1.0f;

	virtual bool CanApplyDelta(float Delta) const override;

	virtual void OnManipulateAxis_Implementation(
		AActor* Interactor,
		float XAxisDelta,
		float YAxisDelta
	) override;
};