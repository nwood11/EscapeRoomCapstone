// Fill out your copyright notice in the Description page of Project Settings.
#include "EscapeGameInstance.h"

// === CORY'S EXISTING ROOM FUNCTIONS ===
void UEscapeGameInstance::MarkRoomComplete(int RoomNumber)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Marking room number %d as complete."), RoomNumber));
	switch (RoomNumber)
	{
	case 1: 
		bRoom1Complete = true;
		break;
	case 2: 
		bRoom2Complete = true;
		break;
	case 3: 
		bRoom3Complete = true;
		break;
	case 4: 
		bRoom4Complete = true;
		break;
	default:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Invalid room number!"));
		break;
	}
}
	
bool UEscapeGameInstance::IsRoomComplete(int RoomNumber)
{
	switch (RoomNumber)
	{
	case 1: return bRoom1Complete;
	case 2: return bRoom2Complete;
	case 3: return bRoom3Complete;
	case 4: return bRoom4Complete;
	default: return false;
	}
}

// === COLIN'S TIMER FUNCTIONS ===
void UEscapeGameInstance::StartTimer()
{
	bTimerRunning = true;
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Timer Started!"));
}

void UEscapeGameInstance::StopTimer()
{
	bTimerRunning = false;
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Timer Stopped!"));
}

void UEscapeGameInstance::UpdateTimer(float DeltaTime)
{
	if (bTimerRunning && RemainingTime > 0)
	{
		RemainingTime -= DeltaTime;
		
		// Broadcast to HUD
		OnTimerUpdated.Broadcast(RemainingTime);
		
		// Check if time ran out
		if (RemainingTime <= 0)
		{
			RemainingTime = 0;
			bTimerRunning = false;
			OnTimerExpired.Broadcast();
			
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Time's Up!"));
		}
	}
}

FString UEscapeGameInstance::GetFormattedTime()
{
	int32 Minutes = FMath::FloorToInt(RemainingTime / 60.0f);
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(RemainingTime, 60.0f));
	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

// === INVENTORY FUNCTIONS (Nico) ===
void UEscapeGameInstance::AddItem(FName ItemName, int32 Quantity, FText Description, UTexture2D* Icon)
{
	if (ItemName.IsNone() || Quantity <= 0)
	{
		return;
	}

	if (FInventoryItem* ExistingItem = Inventory.Find(ItemName))
	{
		if (ExistingItem->MaxStackSize > 0)
		{
			int32 SpaceLeft = ExistingItem->MaxStackSize - ExistingItem->Quantity;
			if (SpaceLeft <= 0)
			{
				return;
			}
			Quantity = FMath::Min(Quantity, SpaceLeft);
		}

		ExistingItem->Quantity += Quantity;
		if (ExistingItem->Icon == nullptr && Icon != nullptr)
		{
			ExistingItem->Icon = Icon;
		}

	}
	else
	{
		FInventoryItem NewItem(ItemName, Quantity);
		if (!Description.IsEmpty())
		{
			NewItem.Description = Description;
		}
		if (Icon != nullptr)
		{
			NewItem.Icon = Icon;
		}
		Inventory.Add(ItemName, NewItem);
	}	

	OnInventoryUpdated.Broadcast(ItemName, Inventory[ItemName].Quantity);
}

bool UEscapeGameInstance::RemoveItem(FName ItemName, int32 Quantity)
{
	if (ItemName.IsNone() || Quantity <= 0)
	{
		return false;
	}

	FInventoryItem* ExistingItem = Inventory.Find(ItemName);
	if (!ExistingItem || ExistingItem->Quantity < Quantity)
	{
		return false;
	} //doesnt allow user to remove items they dont have

	ExistingItem->Quantity -= Quantity;

	if (ExistingItem->Quantity <= 0)
	{
		Inventory.Remove(ItemName);
	} //removes item from inventory if quantity is 0

	OnInventoryUpdated.Broadcast(ItemName, ExistingItem ? ExistingItem->Quantity : 0);
	return true;
}

void UEscapeGameInstance::ClearInventory()
{
	Inventory.Empty();
}

TArray<FInventoryItem> UEscapeGameInstance::GetItems()
{
	TArray<FInventoryItem> ItemsArray;
	
	for (const TPair<FName, FInventoryItem>& Pair : Inventory)
	{
		ItemsArray.Add(Pair.Value);
	}
	
	return ItemsArray;
}

bool UEscapeGameInstance::HasItem(FName ItemName, int32 MinQuantity)
{
	if (FInventoryItem* ExistingItem = Inventory.Find(ItemName))
	{
		return ExistingItem->Quantity >= MinQuantity;
	}
	return false;
}

int32 UEscapeGameInstance::GetItemQuantity(FName ItemName)
{
	if (FInventoryItem* ExistingItem = Inventory.Find(ItemName))
	{
		return ExistingItem->Quantity;
	}
	return 0;
}

FInventoryItem UEscapeGameInstance::GetItemData(FName ItemName)
{
	if (FInventoryItem* ExistingItem = Inventory.Find(ItemName))
	{
		return *ExistingItem;
	}
	
	// Return empty item if not found
	return FInventoryItem();
}
