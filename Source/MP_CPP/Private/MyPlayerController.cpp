#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h" // 添加此行以解决UInputMappingContext不完整类型问题

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!Subsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("EnhancedInput Subsystem nullptr! 大概率是服务端PC，不需要处理输入"));
        return;
    }

    for (UInputMappingContext* Ctx : DefaultMappingContexts)
    {
        if (Ctx != nullptr) // 过滤数组里的None空元素
        {
            Subsystem->AddMappingContext(Ctx, 0);
            UE_LOG(LogTemp, Log, TEXT("成功加载IMC：%s"), *Ctx->GetName());
        }
    }
}
