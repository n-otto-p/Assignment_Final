// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment_FinalGameMode.h"
#include "Assignment_FinalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAssignment_FinalGameMode::AAssignment_FinalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
