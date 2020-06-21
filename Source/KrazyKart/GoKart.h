// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};

UCLASS()
class KRAZYKART_API AGoKart : public APawn
{
	GENERATED_BODY()

	UPROPERTY(ReplicatedUsing=OnRep_ServerState)
	FGoKartState ServerState;

	FVector Velocity;

	UFUNCTION()
	void OnRep_ServerState();
	void MoveCar(float DeltaTime);

	void RotateCar(float DeltaTime, float Steer);

	float Throttle;
	float SteeringThrow;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);

	void SimulateMove(FGoKartMove Move);
	FGoKartMove CreateMove(float DeltaTime);

	void ClearAcknowledgedMoves(FGoKartMove LastMove);

	void MoveForward(float Val);
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

	// Higher means more drag
	UPROPERTY(EditAnywhere)
	float DragCoeff = 16;


	UPROPERTY(EditAnywhere)
	float RollingCoef = 0.022f;

	FVector GetAirResistance();

	FVector GetRollingResistance();

	TArray<FGoKartMove> PendingMoves;

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
