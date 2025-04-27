#pragma once

#include "CoreMinimal.h"
#include "scut/Character/Base/SCUTBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "SCUTPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SCUT_API ASCUTPlayerCharacter : public ASCUTBaseCharacter
{
    GENERATED_BODY()

public:
    ASCUTPlayerCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
    virtual void Tick(float DeltaTime) override;

    // 相机控制
    UFUNCTION(BlueprintCallable, Category = "Player|Camera")
    void SetCameraDistance(float NewDistance);

    UFUNCTION(BlueprintCallable, Category = "Player|Camera")
    void SetCameraRotation(const FRotator& NewRotation);

    // 移动控制
    UFUNCTION(BlueprintCallable, Category = "Player|Movement")
    void SetMovementSpeed(float NewSpeed);

    // 获取玩家输入状态
    UFUNCTION(BlueprintPure, Category = "Player|Input")
    bool IsMoving() const;

    // 重写奔跑状态获取函数
    virtual bool IsRunning() const override;

protected:
    // 输入映射上下文
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
    UInputMappingContext* DefaultMappingContext;

    // 输入动作
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
    UInputAction* RunAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
    UInputAction* JumpAction;

    // 相机臂组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
    USpringArmComponent* CameraBoom;

    // 相机组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
    UCameraComponent* FollowCamera;

    // 移动速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
    float WalkSpeed = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
    float RunSpeed = 600.0f;

    // 跳跃相关
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
    float JumpHeight = 600.0f;

    // 输入状态
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Input")
    bool bIsRunning;

private:
    // 输入处理函数
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartRunning(const FInputActionValue& Value);
    void StopRunning(const FInputActionValue& Value);
    void Jump(const FInputActionValue& Value);
    void StopJumping(const FInputActionValue& Value);
}; 