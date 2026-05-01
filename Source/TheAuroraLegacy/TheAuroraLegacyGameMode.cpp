// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheAuroraLegacyGameMode.h"
#include "TheAuroraLegacyPawn.h"

ATheAuroraLegacyGameMode::ATheAuroraLegacyGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ATheAuroraLegacyPawn::StaticClass();
}
