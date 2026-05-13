#include "ValveGauge.h"
#include "ManipObjectValve.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AValveGauge::AValveGauge()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GaugeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GaugeBody"));
	GaugeBody->SetupAttachment(Root);

	GaugeIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GaugeIndicator"));
	GaugeIndicator->SetupAttachment(Root);

	TargetZoneIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetZoneIndicator"));
	TargetZoneIndicator->SetupAttachment(Root);
}

void AValveGauge::BeginPlay()
{
	Super::BeginPlay();

	for (const FValveGaugeInfluence& Entry : ValveInfluences)
	{
		if (Entry.Valve)
		{
			Entry.Valve->OnValueChanged.AddDynamic(
				this,
				&AValveGauge::OnSourceValveChanged
			);
		}
	}

	RecalculateGauge();
	UpdateTargetZoneIndicator();
}

void AValveGauge::OnSourceValveChanged(float NewValue, float Percentage)
{
	RecalculateGauge();
}

void AValveGauge::RecalculateGauge()
{
	float TotalInfluence = 0.0f;
	float WeightedPercentage = 0.0f;

	for (const FValveGaugeInfluence& Entry : ValveInfluences)
	{
		if (!Entry.Valve || FMath::IsNearlyZero(Entry.Influence))
		{
			continue;
		}

		WeightedPercentage += Entry.Valve->GetTurnPercentage() * Entry.Influence;
		TotalInfluence += Entry.Influence;
	}

	const float FinalPercentage = FMath::Clamp(WeightedPercentage, 0.0f, 1.0f);

	CurrentValue = FMath::Lerp(
		IndicatorMinX,
		IndicatorMaxX,
		FinalPercentage
	);

	if (GaugeIndicator)
	{
		GaugeIndicator->SetRelativeLocation(FVector(CurrentValue, 0.0f, 0.0f));
	}

	const bool bNewTargetMet =
		FMath::Abs(TargetValue - CurrentValue) <= TargetTolerance;

	SetTargetMet(bNewTargetMet);
}

bool AValveGauge::IsTargetMet() const
{
	return bTargetMet;
}

void AValveGauge::SetTargetMet(bool bNewTargetMet)
{
	if (bTargetMet == bNewTargetMet)
	{
		return;
	}

	bTargetMet = bNewTargetMet;
	OnGaugeTargetMetChanged.Broadcast(bTargetMet);
}

void AValveGauge::UpdateTargetZoneIndicator()
{
	if (!TargetZoneIndicator)
	{
		return;
	}

	const float ClampedTargetValue =
		FMath::Clamp(TargetValue, IndicatorMinX, IndicatorMaxX);

	TargetZoneIndicator->SetRelativeLocation(
		FVector(ClampedTargetValue, 0.0f, 0.0f)
	);
}