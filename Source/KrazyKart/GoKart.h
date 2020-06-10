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
	void MoveRight(float Value);

	// The mass of the car in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Force applied to the car when the throttle is fully down N
	UPROPERTY(EditAnywhere)
	float MaxForce = 10000;

	// Number of degrees rotated per second at full control throw degrees/s
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	float Throttle;
	float SteeringThrow;

	void MoveCar(float DeltaTime);

	void RotateCar(float DeltaTime);

	// Higher means more drag
	UPROPERTY(EditAnywhere)
	float dragCoeff = 16;

	FVector GetResistance();


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
