// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKART_API AGoKart : public APawn
{
	GENERATED_BODY()

	FVector Velocity;

	void MoveForward(float Val);

	// The mass of the car in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Force applied to the car when the throttle is fully down N
	UPROPERTY(EditAnywhere)
	float MaxForce = 10000;

	float Throttle;


public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** Handle pressing forwards */

};
