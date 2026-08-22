// Fill out your copyright notice in the Description page of Project Settings.


#include "LAN/UMP_LANMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UMP_LANMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);

	Button_Host->OnClicked.AddDynamic(this, &UMP_LANMenu::HostButtonClicked);
	Button_Join->OnClicked.AddDynamic(this, &UMP_LANMenu::JoinButtonClicked);
}

void UMP_LANMenu::HostButtonClicked()
{
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	GetOwningPlayer()->SetShowMouseCursor(false);

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, HostingLevel, true, TEXT("lister"));
}

void UMP_LANMenu::JoinButtonClicked()
{
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	GetOwningPlayer()->SetShowMouseCursor(false);

	const FString Address = TextBox_IpAddress->GetText().ToString();
	UGameplayStatics::OpenLevel(this, *Address);
}
