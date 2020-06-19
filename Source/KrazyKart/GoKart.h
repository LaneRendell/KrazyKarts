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

	UPROPERTY(ReplicatedUsing=OnRep_ReplicatedTransform)
	FTransform ReplicatedTransform;

	UFUNCTION()
	void OnRep_ReplicatedTransform();


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Val);
	void MoveForward(float Val);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Value);
	void MoveRight(float Value);

	// The mass of the car in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Force applied to the car when the throttle is fully down N
	UPROPERTY(EditAnywhere)
	float MaxForce = 10000;

	// In meters
	UPROPERTY(EditAnywhere)
	float MinimumTurningRadius = 10;

	float Throttle;
	float SteeringThrow;

	void MoveCar(float DeltaTime);

	void RotateCar(float DeltaTime);

	// Higher means more drag
	UPROPERTY(EditAnywhere)
	float DragCoeff = 16;


	UPROPERTY(EditAnywhere)
	float RollingCoef = 0.022f;

	FVector GetAirResistance();

	FVector GetRollingResistance();

	//FString GetEnumText(ENetRole Role);

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
