#include "ManipObjectLever.h"

AManipObjectLever::AManipObjectLever()
{
	DisplayName = FText::FromString(TEXT("Lever"));
	PromptText = FText::FromString(TEXT("Press F to pull"));

	MaxTurnAmount = 90.0f;

	MinMoveVolume = 0.15f;
	MaxMoveVolume = 1.0f;
	MinMovePitch = 0.9f;
	MaxMovePitch = 1.2f;
	FullSpeedDelta = 2.0f;
}

FRotator AManipObjectLever::GetRotationForDelta(float AppliedDelta) const
{
	return FRotator(AppliedDelta * RotationMultiplier, 0.0f, 0.0f);
}

void AManipObjectLever::OnManipulateAxis_Implementation(
	AActor* Interactor,
	float XAxisDelta,
	float YAxisDelta
)
{
	const float Delta = -YAxisDelta * TurnSpeed;

	HandleManipulationDelta(Delta);
}

void AManipObjectLever::OnDeltaApplied(float AppliedDelta)
{
	Super::OnDeltaApplied(AppliedDelta);

	if (CurrentTurnAmount <= MinTurnAmount)
	{
		OnMinReached.Broadcast();
	}
	else if (CurrentTurnAmount >= MaxTurnAmount)
	{
		OnMaxReached.Broadcast();
	}
}