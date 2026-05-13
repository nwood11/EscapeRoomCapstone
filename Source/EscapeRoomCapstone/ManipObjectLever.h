#pragma once

#include "CoreMinimal.h"
#include "ManipObjectRotaryBase.h"
#include "ManipObjectLever.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeverLimitReached);

UCLASS()
class ESCAPEROOMCAPSTONE_API AManipObjectLever : public AManipObjectRotaryBase
{
	GENERATED_BODY()

public:
	AManipObjectLever();

protected:
	virtual FRotator GetRotationForDelta(float AppliedDelta) const override;
	virtual void OnDeltaApplied(float AppliedDelta) override;

public:
	UPROPERTY(BlueprintAssignable, Category="Lever")
	FOnLeverLimitReached OnMinReached;

	UPROPERTY(BlueprintAssignable, Category="Lever")
	FOnLeverLimitReached OnMaxReached;

	virtual void OnManipulateAxis_Implementation(
		AActor* Interactor,
		float XAxisDelta,
		float YAxisDelta
	) override;
};