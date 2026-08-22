// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMP_LANMenu.generated.h"

class UEditableTextBox;
/**
 * 
 */
UCLASS()
class MP_CPP_API UMP_LANMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_IpAddress;
};
