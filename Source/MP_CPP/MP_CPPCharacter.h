// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interaction/MP_Player.h"
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
	UPROPERTY(ReplicatedUsing = OnRep_Armor)
	float Armor = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_PickupCount)
	int PickupCount = 0;

	bool bReplicatePickupCount = true;

	UFUNCTION()
	void OnRep_Armor();

	UFUNCTION()
	void OnRep_PickupCount(int PreviousValue);

};

