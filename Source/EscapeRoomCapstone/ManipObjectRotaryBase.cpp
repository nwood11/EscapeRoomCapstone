#include "ManipObjectRotaryBase.h"
#include "Components/StaticMeshComponent.h"

AManipObjectRotaryBase::AManipObjectRotaryBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RotaryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotaryMesh"));
	SetRootComponent(RotaryMesh);

	MovementAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("MovementAudio"));
	MovementAudio->SetupAttachment(RotaryMesh);
	MovementAudio->bAutoActivate = false;
	MovementAudio->SetVolumeMultiplier(1.0f);
}

void AManipObjectRotaryBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentTurnAmount = FMath::Clamp(CurrentTurnAmount, MinTurnAmount, MaxTurnAmount);
	BroadcastCurrentValue();
}

float AManipObjectRotaryBase::GetTurnPercentage() const
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

float AManipObjectRotaryBase::GetCurrentTurnAmount() const
{
	return CurrentTurnAmount;
}

bool AManipObjectRotaryBase::CanApplyDelta(float Delta) const
{
	const float NewValue = CurrentTurnAmount + Delta;
	return NewValue >= MinTurnAmount && NewValue <= MaxTurnAmount;
}

void AManipObjectRotaryBase::HandleManipulationDelta(float Delta)
{
	if (FMath::IsNearlyZero(Delta))
	{
		StopMovementAudio();
		return;
	}

	if (!CanApplyDelta(Delta))
	{
		StopMovementAudio();
		return;
	}

	ApplyDelta(Delta);
}

void AManipObjectRotaryBase::ApplyDelta(float Delta)
{
	const float OldValue = CurrentTurnAmount;

	CurrentTurnAmount = FMath::Clamp(
		CurrentTurnAmount + Delta,
		MinTurnAmount,
		MaxTurnAmount
	);

	const float AppliedDelta = CurrentTurnAmount - OldValue;

	if (FMath::IsNearlyZero(AppliedDelta))
	{
		StopMovementAudio();
		return;
	}

	if (RotaryMesh)
	{
		RotaryMesh->AddRelativeRotation(GetRotationForDelta(AppliedDelta));
	}

	UpdateMovementAudio(AppliedDelta);
	BroadcastCurrentValue();

	OnDeltaApplied(AppliedDelta);
}

void AManipObjectRotaryBase::BroadcastCurrentValue()
{
	OnValueChanged.Broadcast(CurrentTurnAmount, GetTurnPercentage());
}

FRotator AManipObjectRotaryBase::GetRotationForDelta(float AppliedDelta) const
{
	return FRotator::ZeroRotator;
}

void AManipObjectRotaryBase::OnDeltaApplied(float AppliedDelta)
{
}

void AManipObjectRotaryBase::UpdateMovementAudio(float AppliedDelta)
{
	if (!MovementAudio || !MovementSound)
	{
		return;
	}

	const float SafeFullSpeedDelta = FMath::Max(FullSpeedDelta, KINDA_SMALL_NUMBER);

	const float SpeedAlpha = FMath::Clamp(
		FMath::Abs(AppliedDelta) / SafeFullSpeedDelta,
		0.0f,
		1.0f
	);

	const float Volume = FMath::Lerp(MinMoveVolume, MaxMoveVolume, SpeedAlpha);
	const float Pitch = FMath::Lerp(MinMovePitch, MaxMovePitch, SpeedAlpha);

	if (!MovementAudio->IsPlaying())
	{
		MovementAudio->SetSound(MovementSound);
		MovementAudio->Play();
	}

	MovementAudio->SetVolumeMultiplier(Volume);
	MovementAudio->SetPitchMultiplier(Pitch);
}

void AManipObjectRotaryBase::StopMovementAudio()
{
	if (MovementAudio && MovementAudio->IsPlaying())
	{
		MovementAudio->FadeOut(0.15f, 0.0f);
	}
}

void AManipObjectRotaryBase::OnInteract_Implementation(AActor* Interactor)
{
}

FText AManipObjectRotaryBase::GetInteractDisplayName_Implementation() const
{
	return DisplayName;
}

FText AManipObjectRotaryBase::GetInteractPromptText_Implementation(AActor* Interactor) const
{
	return PromptText;
}

bool AManipObjectRotaryBase::ShouldEnterManipulation_Implementation(AActor* Interactor) const
{
	return true;
}

void AManipObjectRotaryBase::OnBeginManipulation_Implementation(AActor* Interactor)
{
}

void AManipObjectRotaryBase::OnEndManipulation_Implementation(AActor* Interactor)
{
	StopMovementAudio();
}