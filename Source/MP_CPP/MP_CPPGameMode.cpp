// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_CPPGameMode.h"
#include "MP_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMP_CPPGameMode::AMP_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
