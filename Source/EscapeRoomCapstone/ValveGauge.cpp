#include "ValveGauge.h"
#include "ManipObjectValve.h"
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

	if (ConnectedValve)
	{
		ConnectedValve->OnValueChanged.AddDynamic(this, &AValveGauge::UpdateGauge);
		UpdateGauge(ConnectedValve->CurrentTurnAmount, ConnectedValve->GetTurnPercentage());
	}
	UpdateTargetZoneIndicator();
}

void AValveGauge::UpdateGauge(float NewValue, float Percentage)
{
	CurrentValue = FMath::Lerp(IndicatorMinX, IndicatorMaxX, Percentage);

	if (GaugeIndicator)
	{
		GaugeIndicator->SetRelativeLocation(FVector(CurrentValue, 0.0f, 0.0f));
	}

	const bool bNewTargetMet = FMath::Abs(TargetValue - CurrentValue) <= TargetTolerance;
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

	const float ClampedTargetValue = FMath::Clamp(TargetValue, IndicatorMinX, IndicatorMaxX);

	TargetZoneIndicator->SetRelativeLocation(FVector(ClampedTargetValue, 0.0f, 0.0f));
}