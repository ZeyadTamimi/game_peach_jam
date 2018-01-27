// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "powerlinesGameMode.h"
#include "powerlinesCharacter.h"

ApowerlinesGameMode::ApowerlinesGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ApowerlinesCharacter::StaticClass();	
}
