#include "ManipObjectValve.h"

AManipObjectValve::AManipObjectValve()
{
	DisplayName = FText::FromString(TEXT("Valve"));
	PromptText = FText::FromString(TEXT("Press F to turn"));

	MaxTurnAmount = 100.0f;

	MinMoveVolume = 0.05f;
	MaxMoveVolume = 1.0f;
	MinMovePitch = 0.8f;
	MaxMovePitch = 1.25f;
	FullSpeedDelta = 3.0f;
}

FRotator AManipObjectValve::GetRotationForDelta(float AppliedDelta) const
{
	return FRotator(0.0f, 0.0f, -AppliedDelta * RotationMultiplier);
}

bool AManipObjectValve::CanApplyDelta(float Delta) const
{
	if (!Super::CanApplyDelta(Delta))
	{
		return false;
	}

	if (ConnectedValve)
	{
		const float ConnectedDelta = Delta * ConnectedValveMultiplier;

		if (!ConnectedValve->Super::CanApplyDelta(ConnectedDelta))
		{
			return false;
		}
	}

	return true;
}

void AManipObjectValve::OnManipulateAxis_Implementation(
	AActor* Interactor,
	float XAxisDelta,
	float YAxisDelta
)
{
	const float Delta = XAxisDelta * TurnSpeed;

	HandleManipulationDelta(Delta);
}

void AManipObjectValve::OnDeltaApplied(float AppliedDelta)
{
	Super::OnDeltaApplied(AppliedDelta);

	if (ConnectedValve)
	{
		ConnectedValve->ApplyDelta(AppliedDelta * ConnectedValveMultiplier);
	}
}

void AManipObjectValve::BeginPlay()
{
	Super::BeginPlay();

	if (RotaryMesh)
	{
		RotaryMesh->AddRelativeRotation(GetRotationForDelta(CurrentTurnAmount));
	}
}