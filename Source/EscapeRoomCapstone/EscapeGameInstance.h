// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EscapeGameInstance.generated.h"

// Delegate for HUD communication - broadcasts when timer updates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerUpdated, float, RemainingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerExpired);

// Delegate for inventory
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated, FName, ItemName, int32, Quantity);

//inventory data structure
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 Quantity = 0;

	//optional display name
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FText DisplayName;

	//optional description
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FText Description;

	//default constr
	FInventoryItem()
		: ItemName(NAME_None)
		, Quantity(0)
		, DisplayName(FText::GetEmpty())
		, Description(FText::GetEmpty())
	{}

	FInventoryItem(FName InName, int32 InQuantity = 1)
		: ItemName(InName)
		, Quantity(InQuantity)
		, DisplayName(FText::FromName(InName))
		, Description(FText::GetEmpty())
	{}
};

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

	//inventory
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TMap<FName, FInventoryItem> Inventory;

	// === DELEGATES FOR HUD COMMUNICATION ===
	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimerUpdated OnTimerUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimerExpired OnTimerExpired;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

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

	// === INVENTORY FUNCTIONS(Nico) ===
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemName, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemName, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();
};
