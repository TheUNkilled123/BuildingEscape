// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionReporter.h"


// Sets default values for this component's properties
UPositionReporter::UPositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPositionReporter::BeginPlay()
{
	Super::BeginPlay();

	/*FString Log = TEXT("Position reporter reporting from: ");
	Log.Append(GetOwner()->GetName());
	FString* PtrLog = &Log;
	*/
	FVector ActorPos = GetOwner()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Position reporter reporting from: %s!"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("The location of my actor is: X:%f, Y:%f, Z:%f!"), ActorPos.X,ActorPos.Y,ActorPos.Z);
	
}


// Called every frame
void UPositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

