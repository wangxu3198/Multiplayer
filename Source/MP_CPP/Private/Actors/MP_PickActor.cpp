// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MP_PickActor.h"

void AMP_PickActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!OtherActor->Implements<UMP_Player>() || !HasAuthority()) return;

	IMP_Player::Execute_IncrementPickupCount(OtherActor);
	//销毁动作会同步到复制角色
	Destroy();

}
