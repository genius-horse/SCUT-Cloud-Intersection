#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SCUTAnimInstance.generated.h"

UCLASS()
class SCUT_API USCUTAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    USCUTAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    // 动画状态变量
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsMoving;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsRunning;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsJumping;

private:
    // 缓存指向拥有者的指针
    class ASCUTBaseCharacter* CharacterOwner;
}; 