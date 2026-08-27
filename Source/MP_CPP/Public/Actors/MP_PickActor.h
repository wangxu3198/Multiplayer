// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MP_Actor.h"
#include "Interaction/MP_Player.h"
#include "MP_PickActor.generated.h"

/**
 * 
 */
UCLASS()
class MP_CPP_API AMP_PickActor : public AMP_Actor
{
	GENERATED_BODY()
public:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
