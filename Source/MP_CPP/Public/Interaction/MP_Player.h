// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MP_Player.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMP_Player : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MP_CPP_API IMP_Player
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	UFUNCTION(BlueprintNativeEvent)
	void GrantArmor(float ArmorAmount);

	UFUNCTION(BlueprintNativeEvent)
	void IncrementPickupCount();
};
