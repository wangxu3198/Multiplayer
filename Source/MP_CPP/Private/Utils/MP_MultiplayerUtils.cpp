// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MP_MultiplayerUtils.h"

void UMP_MultiplayerUtils::PrintLocalNetRole(AActor* Actor)
{
	if (Actor->HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Has Authority."), *Actor->GetName())
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Does Not Have Authority."), *Actor->GetName())
	}

    switch (Actor->GetLocalRole())
	{
	case ROLE_None:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Local Role: ROLE_None"), *Actor->GetName());
		break;
	case ROLE_SimulatedProxy:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Local Role: ROLE_SimulatedProxy"), *Actor->GetName());
		break;
	case ROLE_AutonomousProxy:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Local Role: ROLE_AutonomousProxy"), *Actor->GetName());
		break;
	case ROLE_Authority:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Local Role: ROLE_Authority"), *Actor->GetName());
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Local Role: Unknown (%d)"), *Actor->GetName(), static_cast<int32>(Actor->GetLocalRole()));
		break;
	}
}

void UMP_MultiplayerUtils::PrintRemoteNetRole(AActor* Actor)
{
    if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("PrintRemoteNetRole called with null Actor."));
		return;
	}

	if (Actor->HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Has Authority."), *Actor->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Does Not Have Authority."), *Actor->GetName());
	}

	switch (Actor->GetRemoteRole())
	{
	case ROLE_None:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Remote Role: ROLE_None"), *Actor->GetName());
		break;
	case ROLE_SimulatedProxy:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Remote Role: ROLE_SimulatedProxy"), *Actor->GetName());
		break;
	case ROLE_AutonomousProxy:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Remote Role: ROLE_AutonomousProxy"), *Actor->GetName());
		break;
	case ROLE_Authority:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Remote Role: ROLE_Authority"), *Actor->GetName());
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Actor %s Remote Role: Unknown (%d)"), *Actor->GetName(), static_cast<int32>(Actor->GetRemoteRole()));
		break;
	}
}
