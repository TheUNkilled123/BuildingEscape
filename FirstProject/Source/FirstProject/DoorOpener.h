// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "DoorOpener.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPROJECT_API UDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:

	float CurrentPos;
	float NewPos;
	float LastTotalWeight = 0.f;
	float CurrentTotalMass;
	FRotator InitialRotation;
	FRotator TargetRotation;
	FRotator CurrentRotation;
	FRotator NewRotation;
	float TotalMassOfActors() const;
	virtual void UpdatePhysicalPressurePlate();
	float mapRange(float a1, float a2, float b1, float b2, float s);
	void FindAudioComponent();
	void FindPressurePlate();
	bool bDoorLastStateIsOpen = false;


	UPROPERTY()
	UAudioComponent* PressurePlateAudioComponent = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* PhysicalPressurePlateActor = nullptr;
	
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
	float TargetYaw = -90.f;

	UPROPERTY(EditAnywhere)
	//Time in seconds that defines how long the doors stay open after the player steps off the pressure plate.
	float DoorOpenTime = 0.45f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	float PressurePlateActorMaxZ = 90.f;

	UPROPERTY(EditAnywhere)
	float PressurePlateActorMinZ = 73.f;
};
