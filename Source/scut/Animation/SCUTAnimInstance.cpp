#include "scut/Animation/SCUTAnimInstance.h"
#include "scut/Character/Base/SCUTBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USCUTAnimInstance::USCUTAnimInstance()
{
    Speed = 0.0f;
    bIsMoving = false;
    bIsRunning = false;
    bIsJumping = false;
    CharacterOwner = nullptr;
}

void USCUTAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // 获取角色引用
    if (AActor* Owner = GetOwningActor())
    {
        CharacterOwner = Cast<ASCUTBaseCharacter>(Owner);
    }
}

void USCUTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!CharacterOwner)
    {
        return;
    }

    // 更新速度
    Speed = CharacterOwner->GetCharacterSpeed();
    
    // 更新移动状态
    bIsMoving = Speed > 0.0f;
    
    // 更新奔跑状态
    bIsRunning = CharacterOwner->IsRunning();

    // 更新跳跃状态
    if (UCharacterMovementComponent* MovementComponent = CharacterOwner->GetCharacterMovement())
    {
        bIsJumping = MovementComponent->IsFalling();
    }
} 