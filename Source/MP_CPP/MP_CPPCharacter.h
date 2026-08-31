// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interaction/MP_Player.h"
#include "Components/MP_HealthComponent.h"
#include "MP_CPPCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMP_CPPCharacter : public ACharacter, public IMP_Player
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AMP_CPPCharacter();
	
	virtual USkeletalMeshComponent* GetSkeletalMeshComponent_Implementation() const override;

	virtual void GrantArmor_Implementation(float ArmorAmount) override;

	virtual void IncrementPickupCount_Implementation() override;

	virtual void Jump() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void OnRPCDelayTimer();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//复制Armor属性值的步骤
	/*
		1.override GetLifetimeReplicatedProps函数（public）
		2.U属性宏带复制标记(UPROPERTY(Replicated))
		3.在get lifetime replicated props中调用宏执行do rep lifetime(#include "Net/UnrealNetwork.h")
	*/

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//自定义条件
	/*
		1.设定一个bool变量
		2.override PreReplication函数
		3.PreReplication中调用宏DOREPLIFETIME_ACTIVE_OVERRIDE（需要NetCore模块）

		注意：
			1.bool值仅在服务器上有效，因为PreReplication是服务器才调用的函数
			2.当停止复制时更改值，之后在恢复复制时服务器值将同步并修正客户端版本的值
			3.这个功能非常强大，如果频繁变化可能会很耗资源
	*/

	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

private:
	FTimerHandle RPCDelayTimer;

	UPROPERTY(ReplicatedUsing = OnRep_Armor)
	float Armor = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_PickupCount)
	int PickupCount = 0;

	bool bReplicatePickupCount = true;

	UFUNCTION()
	void OnRep_Armor();

	UFUNCTION()
	void OnRep_PickupCount(int PreviousValue);


	//复制组件和变量不同
	/*
		多种方式实现该复制
		1.进入健康组件，在其构造函数中设置复制属性SetIsReplicatedByDefault(true)
		2.在创建他的地方调用SetIsReplicated(true)
		3.蓝图中的Component Replicates

		注：**`CreateDefaultSubobject` 创建出来的组件，Owner 自动就是当前这个 `AMP_CPPCharacter`（你的 Character Actor）**。
	*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMP_HealthComponent> HealthComponent;

	//服务器上调用一个函数，并让它在拥有者客户端执行
	/*
		一个可复制函数也称为远程过程调用简称RPC
		RPC(Remote Procedure Call)三种类型：
			1.Client
			2.Server
			3.NetMulticast:网络多播

		客户端RPC
		UFUNCTION(Client, Reliable)
			Reliable:可靠的。它将在远程机器上保证执行
			
	*/

	//它们可以有输入参数，因此可以接收数据。这些数据将通过网络传输
	//Client_PrintMessage_Implementation：实现的名
	UFUNCTION(Client, Reliable)
	void Client_PrintMessage(const FString& Message);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PrintMessage(const FString& Message);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PrintMessage(const FString& Message);
};

