// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_CPPCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include <Actors/MP_Actor.h>


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMP_CPPCharacter

AMP_CPPCharacter::AMP_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	HealthComponent = CreateDefaultSubobject<UMP_HealthComponent>("HealthComponent");
	HealthComponent->SetIsReplicated(true);
}

USkeletalMeshComponent* AMP_CPPCharacter::GetSkeletalMeshComponent_Implementation() const
{
	return GetMesh();
}

void AMP_CPPCharacter::GrantArmor_Implementation(float ArmorAmount)
{
	Armor = ArmorAmount;
}

void AMP_CPPCharacter::IncrementPickupCount_Implementation()
{
	PickupCount++;
}

void AMP_CPPCharacter::Jump()
{
	Super::Jump();

	//Server RPC
	Server_PrintMessage("This should run on owning Server.");

	if (!HasAuthority()) return;
	bReplicatePickupCount = !bReplicatePickupCount;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("bReplicatePickupCount: %d"), bReplicatePickupCount));
}

void AMP_CPPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GetWorldTimerManager().SetTimer(RPCDelayTimer, this, &AMP_CPPCharacter::OnRPCDelayTimer, 4.0f, false);

}

void AMP_CPPCharacter::OnRPCDelayTimer()
{
	//在Begin中执行时都在服务器运行
	//有时开始播放太早，还没有拥有网络连接。我们必须确保已建立连接，以确保RPC正常工作。
	//具体方法取决于游戏和玩法机制，我们将简单设置一个计时器，在八秒后运行此函数
	/*if (HasAuthority()) {

		Client_PrintMessage("This should run on owning client.");
	}*/

	/*if (!HasAuthority()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	GetWorld()->SpawnActor<AMP_Actor>(GetActorLocation(), GetActorRotation(), SpawnParams);*/
	
	//注意Host（监听服务器下的主机）HasAuthority() == true
	/*if (!HasAuthority()) {

		Server_PrintMessage("This should run on owning Server.");
	}*/

	if (HasAuthority()) {

		Multicast_PrintMessage("This should run on owning Server and all relevant client.");
	}
}

void AMP_CPPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AMP_CPPCharacter, Armor, COND_AutonomousOnly);
	//DOREPLIFETIME(AMP_CPPCharacter, PickupCount);
	DOREPLIFETIME_CONDITION(AMP_CPPCharacter, PickupCount, COND_Custom);
}

void AMP_CPPCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	//建立bool变量和复制变量的关系（复制条件的自定义条件）
	//bReplicatePickupCount = true则复制，= false则不复制
	//若想让该宏起作用还需添加NetCore模块
	DOREPLIFETIME_ACTIVE_OVERRIDE(AMP_CPPCharacter, PickupCount, bReplicatePickupCount)
}

void AMP_CPPCharacter::OnRep_Armor()
{
	//RepNotify = “收到网络同步数据包之后” 的回调。谁收到同步包，谁执行；本机直接改值，不触发。
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Armor: %f"), Armor));
}

void AMP_CPPCharacter::OnRep_PickupCount(int PreviousValue)
{
	//int类型的复制通知函数可以带参数，参数是同步前的值。
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PreviousValue: %d"), PreviousValue));
	//RepNotify = “收到网络同步数据包之后” 的回调。谁收到同步包，谁执行；本机直接改值，不触发。
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PickupCount: %d"), PickupCount));
}

void AMP_CPPCharacter::Multicast_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server" : "Client";
	MessageString += Message;

	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Purple, MessageString);
}

void AMP_CPPCharacter::Server_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server" : "Client";
	MessageString += Message;

	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Purple, MessageString);
}

void AMP_CPPCharacter::Client_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server" : "Client";
	MessageString += Message;

	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, MessageString);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMP_CPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMP_CPPCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMP_CPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMP_CPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}