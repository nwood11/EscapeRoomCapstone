#include "ManipObjectValve.h"
#include "Components/StaticMeshComponent.h"

AManipObjectValve::AManipObjectValve()
{
	PrimaryActorTick.bCanEverTick = false;

	ValveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Interactive_Crank_Wheel"));
	SetRootComponent(ValveMesh);
	
	ValveTurningAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ValveTurningAudio"));
	ValveTurningAudio->SetupAttachment(ValveMesh);
	ValveTurningAudio->bAutoActivate = false;
	ValveTurningAudio->SetVolumeMultiplier(1.0f);
}

void AManipObjectValve::BeginPlay()
{
	Super::BeginPlay();

	CurrentTurnAmount = FMath::Clamp(CurrentTurnAmount, MinTurnAmount, MaxTurnAmount);
	BroadcastCurrentValue();
}

float AManipObjectValve::GetTurnPercentage() const
{
	if (FMath::IsNearlyEqual(MaxTurnAmount, MinTurnAmount))
	{
		return 0.0f;
	}

	return FMath::Clamp(
		(CurrentTurnAmount - MinTurnAmount) / (MaxTurnAmount - MinTurnAmount),
		0.0f,
		1.0f
	);
}

bool AManipObjectValve::CanApplyDelta(float Delta) const
{
	const float NewValue = CurrentTurnAmount + Delta;

	if (NewValue < MinTurnAmount || NewValue > MaxTurnAmount)
	{
		return false;
	}

	if (ConnectedValve)
	{
		const float ConnectedDelta = Delta * ConnectedValveMultiplier;
		const float ConnectedNewValue = ConnectedValve->CurrentTurnAmount + ConnectedDelta;

		if (ConnectedNewValue < ConnectedValve->MinTurnAmount ||
			ConnectedNewValue > ConnectedValve->MaxTurnAmount)
		{
			return false;
		}
	}

	return true;
}

void AManipObjectValve::ApplyValveDelta(float Delta)
{
	CurrentTurnAmount = FMath::Clamp(
		CurrentTurnAmount + Delta,
		MinTurnAmount,
		MaxTurnAmount
	);

	const float RollAmount = -Delta * RotationMultiplier;

	if (ValveMesh)
	{
		ValveMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, RollAmount));
	}

	UpdateValveAudio(Delta);
	BroadcastCurrentValue();
}

void AManipObjectValve::BroadcastCurrentValue()
{
	OnValueChanged.Broadcast(CurrentTurnAmount, GetTurnPercentage());
}

FText AManipObjectValve::GetInteractDisplayName_Implementation() const
{
	return DisplayName;
}

FText AManipObjectValve::GetInteractPromptText_Implementation(AActor* Interactor) const
{
	return PromptText;
}

bool AManipObjectValve::ShouldEnterManipulation_Implementation(AActor* Interactor) const
{
	return true;
}

void AManipObjectValve::OnManipulateAxis_Implementation(AActor* Interactor, float XAxisDelta, float YAxisDelta)
{
	if (FMath::IsNearlyZero(XAxisDelta))
	{
		return;
	}

	const float Delta = XAxisDelta * TurnSpeed;

	if (!CanApplyDelta(Delta))
	{
		return;
	}

	ApplyValveDelta(Delta);

	if (ConnectedValve)
	{
		ConnectedValve->ApplyValveDelta(Delta * ConnectedValveMultiplier);
	}
}

void AManipObjectValve::OnEndManipulation_Implementation(AActor* Interactor)
{
	StopValveAudio();
}

void AManipObjectValve::UpdateValveAudio(float AppliedDelta)
{
	if (!ValveTurningAudio || !ValveTurningSound)
	{
		return;
	}

	const float SpeedAlpha = FMath::Clamp(
		FMath::Abs(AppliedDelta) / FullSpeedDelta,
		0.0f,
		1.0f
	);

	const float Volume = FMath::Lerp(MinTurnVolume, MaxTurnVolume, SpeedAlpha);
	const float Pitch = FMath::Lerp(MinTurnPitch, MaxTurnPitch, SpeedAlpha);

	if (!ValveTurningAudio->IsPlaying())
	{
		ValveTurningAudio->SetSound(ValveTurningSound);
		ValveTurningAudio->Play();
	}

	ValveTurningAudio->SetVolumeMultiplier(Volume);
	ValveTurningAudio->SetPitchMultiplier(Pitch);
}

void AManipObjectValve::StopValveAudio()
{
	if (ValveTurningAudio && ValveTurningAudio->IsPlaying())
	{
		ValveTurningAudio->FadeOut(0.15f, 0.0f);
	}
}