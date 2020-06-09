// Copyright Epic Games, Inc. All Rights Reserved.

#include "KrazyKartGameMode.h"
#include "KrazyKartPawn.h"
#include "KrazyKartHud.h"

AKrazyKartGameMode::AKrazyKartGameMode()
{
	DefaultPawnClass = AKrazyKartPawn::StaticClass();
	HUDClass = AKrazyKartHud::StaticClass();
}
