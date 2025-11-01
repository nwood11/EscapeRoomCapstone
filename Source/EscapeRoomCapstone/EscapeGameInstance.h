// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EscapeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOMCAPSTONE_API UEscapeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	bool bRoom1Complete = false;
	bool bRoom2Complete = false;
	bool bRoom3Complete = false;
	bool bRoom4Complete = false;

public:
	UFUNCTION(BlueprintCallable)
	void MarkRoomComplete(int RoomNumber);
	
	UFUNCTION(BlueprintCallable)
	bool IsRoomComplete(int RoomNumber);
	
};
