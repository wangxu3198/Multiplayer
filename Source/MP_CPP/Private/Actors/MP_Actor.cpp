// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MP_Actor.h"

// Sets default values
AMP_Actor::AMP_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bNetLoadOnClient = true;
	bReplicates = true;

	/*
		C++构造函数内部不要调用虚函数(核心原理)
		构造阶段,子类对象还没构造完成,虚表还没切换到子类版本,虚函数只会执行当前类的实现,不会多态。
		SetReplicateMovement(true);
	*/
	SetReplicatingMovement(true);
	
}

// Called when the game starts or when spawned
void AMP_Actor::BeginPlay()
{
	Super::BeginPlay();
	const bool bAuth = HasAuthority();
	//Get NetRole
	const ENetRole localRole = GetLocalRole();

}

// Called every frame
void AMP_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

