// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MP_HealthComponent.h"

// Sets default values for this component's properties
UMP_HealthComponent::UMP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	// SetIsReplicated() 不能在组件构造函数里调用。
	// 在创建他的地方调用
	//SetIsReplicated(true);
	SetIsReplicatedByDefault(true);//这个可以
}


// Called when the game starts
void UMP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMP_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

