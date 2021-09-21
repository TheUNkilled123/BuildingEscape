// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorOpener.h"
#include "MultipleDoorOpener.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRSTPROJECT_API UMultipleDoorOpener : public UDoorOpener
{
	GENERATED_BODY()
	
public:
	void UpdatePhysicalPressurePlate() override;
};
