// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeGameInstance.h"

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