// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpener.h"
#include "Components/AudioComponent.h"
#include "Math/Quat.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	//Get initial door values and calculate the TargetYaw from the InitialYaw + the angle set in the editor.
	InitialRotation = GetOwner()->GetActorRotation();
	TargetYaw += InitialRotation.Yaw;
	if (TargetYaw >= 180.f)
	{
		TargetYaw = -180 + (TargetYaw - 180);
	}
	TargetRotation.Yaw = TargetYaw;

	//Check if a Pressure Plate and AudioComponent are set
	FindPressurePlate();
	FindAudioComponent();
}


// Called every frame
void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTotalMass = TotalMassOfActors();
	//If the total mass of Actors on the Pressure Plate is greater than 50kg, open the door.
	if (CurrentTotalMass > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorOpenTime)
		{
			CloseDoor(DeltaTime);
		}
	}
	//Change the position of the physical Pressure Plate actor(the static mesh of the Pressure Plate)
	UpdatePhysicalPressurePlate();
}


void UDoorOpener::OpenDoor(float DeltaTime)
{
	//If the NewRotation is over 180(will cause the in-engine value to go into negatives) so use RInterp which automatically does the necessary clamping for rotations.
	CurrentRotation = GetOwner()->GetActorRotation();
	NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRelativeRotation(NewRotation);

	if (!bDoorLastStateIsOpen)
	{
		AudioComponent->Play();
		bDoorLastStateIsOpen = true;
	}
}


void UDoorOpener::CloseDoor(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	NewRotation = FMath::RInterpTo(CurrentRotation, InitialRotation, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRelativeRotation(NewRotation);

	if (bDoorLastStateIsOpen)
	{
		AudioComponent->Play();
		bDoorLastStateIsOpen = false;
	}
}


float UDoorOpener::TotalMassOfActors() const
{
	//Initialise values
	float CurrentMass = 0.f;
	TArray<AActor*> OverlappingActors;
	//Get all actors currently on pressure plate.
	PressurePlate->GetOverlappingActors(OverlappingActors);
	//Iterate through all the actors and get their masses.
	for (int i = 1;i <= OverlappingActors.Num();i++)
	{
		CurrentMass += OverlappingActors[i-1]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return CurrentMass;
}


void UDoorOpener::UpdatePhysicalPressurePlate()
{
	CurrentPos = PhysicalPressurePlateActor->GetActorLocation().Z;
	NewPos = FMath::FInterpTo(CurrentPos, mapRange(0.f, 60.f, PressurePlateActorMaxZ, PressurePlateActorMinZ, TotalMassOfActors()), GetWorld()->GetDeltaSeconds(), 2.f);
	FVector FinalPos = PhysicalPressurePlateActor->GetActorLocation();
	FinalPos.Z = NewPos;
	PhysicalPressurePlateActor->SetActorLocation(FinalPos);
	if (CurrentTotalMass != LastTotalWeight)
	{
		PressurePlateAudioComponent->Play();
		LastTotalWeight = CurrentTotalMass;
	}
}


//Function for mapping 2 ranges
float UDoorOpener::mapRange(float a1, float a2, float b1, float b2, float s)
{
	float result = b1 + (s - a1) * (b2 - b1) / (a2 - a1);
	if (result < PressurePlateActorMinZ)
	{
		return PressurePlateActorMinZ;
	}
	return result;
}


void UDoorOpener::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	TArray<AActor*> ChildActor;
	PressurePlate->GetAttachedActors(ChildActor);
	PressurePlateAudioComponent = ChildActor[0]->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("WARNING: Audio component not found on Actor: %s"), *(GetOwner()->GetName()));
	}
	if (!PressurePlateAudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("WARNING: Audio component not found on Actor: %s"), *(PressurePlate->GetName()));
	}
}


void UDoorOpener::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the Door Opener component on it, but it does not have a valid Pressure Plate value set(Nullpntr on Pressure Plate)"), *GetOwner()->GetName());
	}
}