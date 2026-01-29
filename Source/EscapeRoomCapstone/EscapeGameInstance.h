// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EscapeGameInstance.generated.h"

// Delegate for HUD communication - broadcasts when timer updates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerUpdated, float, RemainingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerExpired);

UCLASS()
class ESCAPEROOMCAPSTONE_API UEscapeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// === ROOM COMPLETION (Cory's existing code) ===
	bool bRoom1Complete = false;
	bool bRoom2Complete = false;
	bool bRoom3Complete = false;
	bool bRoom4Complete = false;

	// === TIMER VARIABLES (Colin's additions) ===
	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	float TotalGameTime = 3600.0f;  // 60 minutes in seconds

	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	float RemainingTime = 3600.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	bool bTimerRunning = false;

	// === DELEGATES FOR HUD COMMUNICATION ===
	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimerUpdated OnTimerUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimerExpired OnTimerExpired;

public:
	// === ROOM FUNCTIONS (Cory's existing code) ===
	UFUNCTION(BlueprintCallable)
	void MarkRoomComplete(int RoomNumber);
	
	UFUNCTION(BlueprintCallable)
	bool IsRoomComplete(int RoomNumber);

	// === TIMER FUNCTIONS (Colin's additions) ===
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StopTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void UpdateTimer(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Timer")
	FString GetFormattedTime();
};
