#include "ManipObjectLever.h"
#include "Components/StaticMeshComponent.h"

AManipObjectLever::AManipObjectLever()
{
	PrimaryActorTick.bCanEverTick = false;

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Interactive_Lever"));
	SetRootComponent(LeverMesh);
	
	LeverMovingAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("LeverMovingAudio"));
	LeverMovingAudio->SetupAttachment(LeverMesh);
	LeverMovingAudio->bAutoActivate = false;
	LeverMovingAudio->SetVolumeMultiplier(1.0f);
}

void AManipObjectLever::BeginPlay()
{
	Super::BeginPlay();

	CurrentTurnAmount = FMath::Clamp(CurrentTurnAmount, MinTurnAmount, MaxTurnAmount);
	BroadcastCurrentValue();
}

float AManipObjectLever::GetTurnPercentage() const
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

bool AManipObjectLever::CanApplyDelta(float Delta) const
{
	const float NewValue = CurrentTurnAmount + Delta;
	return NewValue >= MinTurnAmount && NewValue <= MaxTurnAmount;
}

void AManipObjectLever::ApplyLeverDelta(float Delta)
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
		StopLeverAudio();
		return;
	}

	if (LeverMesh)
	{
		const float PitchAmount = AppliedDelta * RotationMultiplier;
		LeverMesh->AddRelativeRotation(FRotator(PitchAmount, 0.0f, 0.0f));
	}

	UpdateLeverAudio(AppliedDelta);
	BroadcastCurrentValue();
}

void AManipObjectLever::BroadcastCurrentValue()
{
	OnValueChanged.Broadcast(CurrentTurnAmount, GetTurnPercentage());
}

FText AManipObjectLever::GetInteractDisplayName_Implementation() const
{
	return DisplayName;
}

FText AManipObjectLever::GetInteractPromptText_Implementation(AActor* Interactor) const
{
	return PromptText;
}

bool AManipObjectLever::ShouldEnterManipulation_Implementation(AActor* Interactor) const
{
	return true;
}

void AManipObjectLever::OnManipulateAxis_Implementation(AActor* Interactor, float XAxisDelta, float YAxisDelta)
{
	if (FMath::IsNearlyZero(YAxisDelta))
	{
		StopLeverAudio();
		return;
	}

	const float Delta = -YAxisDelta * TurnSpeed;

	if (!CanApplyDelta(Delta))
	{
		StopLeverAudio();
		return;
	}

	ApplyLeverDelta(Delta);
}

void AManipObjectLever::OnEndManipulation_Implementation(AActor* Interactor)
{
	StopLeverAudio();
}

void AManipObjectLever::UpdateLeverAudio(float AppliedDelta)
{
	if (!LeverMovingAudio || !LeverMovingSound)
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

	if (!LeverMovingAudio->IsPlaying())
	{
		LeverMovingAudio->SetSound(LeverMovingSound);
		LeverMovingAudio->Play();
	}

	LeverMovingAudio->SetVolumeMultiplier(Volume);
	LeverMovingAudio->SetPitchMultiplier(Pitch);
}

void AManipObjectLever::StopLeverAudio()
{
	if (LeverMovingAudio && LeverMovingAudio->IsPlaying())
	{
		LeverMovingAudio->FadeOut(0.15f, 0.0f);
	}
}