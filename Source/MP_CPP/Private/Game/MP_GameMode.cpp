// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MP_GameMode.h"
#include <Game/MP_GameState.h>

AMP_GameMode::AMP_GameMode()
{
	bDelayedStart = true;

}

void AMP_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_StartMatch, this, &AMP_GameMode::StartMatchTimer, 5.0f, false);
}

void AMP_GameMode::StartMatch()
{
	Super::StartMatch();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Match Started!"));
}

void AMP_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AMP_GameState* MP_GameState = GetGameState<AMP_GameState>();
	if (IsValid(MP_GameState)) {
		MP_GameState->AddPlayerToTeam(NewPlayer);
	}
}

void AMP_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

}

void AMP_GameMode::StartMatchTimer()
{
	StartMatch();
}
