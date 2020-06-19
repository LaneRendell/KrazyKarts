// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGoKart::AGoKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_Authority:
		return "Authority";
	case ROLE_AutonomousProxy:
		return "AutonmousProxy";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	default:
		break;
	}

	return "Error";
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoKart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGoKart, ReplicatedTransform);
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCar(DeltaTime);

	if (HasAuthority())
	{
		ReplicatedTransform = GetActorTransform();
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(this->GetLocalRole()), this, FColor::Green, DeltaTime);
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);

}


void AGoKart::OnRep_ReplicatedTransform()
{
	SetActorTransform(ReplicatedTransform);
}

void AGoKart::Server_MoveForward_Implementation(float Value)
{
	Throttle = Value;
}

bool AGoKart::Server_MoveForward_Validate(float Value)
{
	return FMath::Abs(Value) <= 1;
}

void AGoKart::Server_MoveRight_Implementation(float Value)
{
	SteeringThrow = Value;
}

bool AGoKart::Server_MoveRight_Validate(float Value)
{
	return FMath::Abs(Value) <= 1;

}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
	this->Server_MoveForward(Value);
}

void AGoKart::MoveRight(float Value)
{
	SteeringThrow = Value;
	this->Server_MoveRight(Value);
}

void AGoKart::MoveCar(float DeltaTime)
{

	FVector Force = GetActorForwardVector() * MaxForce * Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

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
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = (DeltaLocation / MinimumTurningRadius) * SteeringThrow;

	FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);


	AddActorWorldRotation(RotationDelta);
}

FVector AGoKart::GetAirResistance()
{
	return -(Velocity.GetSafeNormal()) * Velocity.SizeSquared() * DragCoeff;
}

FVector AGoKart::GetRollingResistance()
{
	float Gravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Gravity * Mass;
	return -(Velocity.GetSafeNormal()) * NormalForce * RollingCoef;
}

