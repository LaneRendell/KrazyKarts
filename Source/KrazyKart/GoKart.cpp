// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
AGoKart::AGoKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoKart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGoKart, ServerState);
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		FGoKartMove Move = CreateMove(DeltaTime);
		// TODO: Set time


		if (!HasAuthority())
		{
			PendingMoves.Add(Move);
			UE_LOG(LogTemp, Warning, TEXT("Queue Length: %d"), PendingMoves.Num());
		}

		Server_SendMove(Move);
		SimulateMove(Move);
	}
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight); 
}


void AGoKart::OnRep_ServerState()
{
	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;
	ClearAcknowledgedMoves(ServerState.LastMove);
}

void AGoKart::Server_SendMove_Implementation(FGoKartMove Move)
{
	SimulateMove(Move);

	ServerState.LastMove = Move;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;
}

bool AGoKart::Server_SendMove_Validate(FGoKartMove Move)
{
	return true; /// TODO: Make better validation
}

void AGoKart::SimulateMove(FGoKartMove Move)
{

	FVector Force = GetActorForwardVector() * MaxForce * Move.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + (Acceleration * Move.DeltaTime);

	RotateCar(Move.DeltaTime, Move.SteeringThrow);

	FVector Translation = Velocity * Move.DeltaTime * 100;

	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

FGoKartMove AGoKart::CreateMove(float DeltaTime)
{
	FGoKartMove Move;
	AGameStateBase* GameState;

	GameState = GetWorld()->GetGameState();

	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GameState->GetServerWorldTimeSeconds();

	return Move;
}

void AGoKart::ClearAcknowledgedMoves(FGoKartMove LastMove)
{
	TArray<FGoKartMove> NewMoves;

	for (const FGoKartMove& Move: PendingMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	PendingMoves = NewMoves;
}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
	//Server_SendMove(Value);
}

void AGoKart::MoveRight(float Value)
{
	SteeringThrow = Value;
}


void AGoKart::RotateCar(float DeltaTime, float Steer)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = (DeltaLocation / MinimumTurningRadius) * Steer;

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

