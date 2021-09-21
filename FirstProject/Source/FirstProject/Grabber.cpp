// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h" //Used only for drawing debug lines, can be removed when optimizing!!!
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Get required component pointers from our pawn
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	//Check if the PhysicsHandleComponent exists(The check can be removed, the pawn will never spawn without one.)
	if(PhysicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found on Pawn: %s!"),*(GetOwner()->GetName()));//NOTE: The * in front of the GetName() is an overload operator, not a pointer.
	}

	//Bind user input "Grab" to functions Grab() and Release().
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}




// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the physics handle currently has an Actor or object attached to it(we're holding it) then update its position every frame.
	if(PhysicsHandle->GetGrabbedComponent())
	{
		//Calculate the "LineTraceEnd" which is the point to where we want the ray to be cast/object to be held from our viewpoint position + Reach.
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewpointPos,
			PlayerViewpointRotation
		);
		
		LineTraceEnd = PlayerViewpointPos + PlayerViewpointRotation.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}




//Called when the player presses the Grab button.
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button pressed!"));

	//Gets player's viewpoint and calculates the LineTraceVector(where to cast the ray).
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewpointPos,
		PlayerViewpointRotation
	);

	LineTraceEnd = PlayerViewpointPos + PlayerViewpointRotation.Vector() * Reach;

	//Gets the first physics body in reach.
	//First the params for the raycast are setup and then the line itself is cast 
	//The hit actor pointer is returned in the "Hit" variable.
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointPos,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	/*DrawDebugLine(
		GetWorld(),
		PlayerViewpointPos,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		5.f
	);*/
	//Check if ActorHit is a null pointer.(This saves us from a crash if nothing is caught by the ray.)
	if (Hit.GetActor())
	{
		//Debug log, can be removed
		UE_LOG(LogTemp, Warning, TEXT("The actor hit by raycast is: %s."), *(Hit.GetActor()->GetName()));
		//Grab the hit actor.(The PhysicsHandle needs a component of the actor so we just get it from the Hit.GetComponent() ).
		PhysicsHandle->GrabComponentAtLocation
		(
			Hit.GetComponent(),
			NAME_None,
			LineTraceEnd
		);
	}
}



//Called when the player releases the "Grab" button.
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button released!"));
	PhysicsHandle->ReleaseComponent();
}