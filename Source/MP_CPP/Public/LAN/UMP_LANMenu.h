// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMP_LANMenu.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class MP_CPP_API UMP_LANMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	// <-- UUserWidget -->
	virtual void NativeOnInitialized() override;
	// <-- !UUserWidget -->

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_IpAddress;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Host;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Join;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> HostingLevel;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();
};
