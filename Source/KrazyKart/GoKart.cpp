// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCar(DeltaTime);
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);


}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}

void AGoKart::MoveRight(float Value)
{
	SteeringThrow = Value;
}

void AGoKart::MoveCar(float DeltaTime)
{

	FVector Force = GetActorForwardVector() * MaxForce * Throttle;

	Force += GetResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + (Acceleration * DeltaTime);

	RotateCar(DeltaTime);


	FVector Delta = Velocity * DeltaTime * 100;

	FHitResult HitResult;

	AddActorWorldOffset(Delta, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("0 Velocity"));
	}
}

void AGoKart::RotateCar(float DeltaTime)
{
	float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));
	RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}

FVector AGoKart::GetResistance()
{
	return -(Velocity.GetSafeNormal()) * Velocity.SizeSquared() * dragCoeff;
}

