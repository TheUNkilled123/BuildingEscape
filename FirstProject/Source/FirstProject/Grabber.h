// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPROJECT_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	FVector PlayerViewpointPos;
	FRotator PlayerViewpointRotation;

	float Reach = 150.f;
	FVector LineTraceEnd;

	FHitResult Hit;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;
		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Grab();
	void Release();

};
