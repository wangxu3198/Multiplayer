#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

// 前向声明
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class MP_CPP_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    // 重写SetupInputComponent，必须声明！你现在头文件**缺失这个函数声明**，这是核心错误
    virtual void SetupInputComponent() override;
};
