#include "scut/Character/Player/SCUTPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASCUTPlayerCharacter::ASCUTPlayerCharacter()
{
    // 设置角色旋转
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // 配置角色移动
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // 创建相机臂
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // 创建相机
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // 初始化状态
    bIsRunning = false;
}

void ASCUTPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    SetMovementSpeed(WalkSpeed);

    // 添加输入映射上下文
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ASCUTPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 根据移动状态更新角色状态
    if (GetVelocity().Size() > 0.0f)
    {
        UpdateCharacterState(bIsRunning ? ECharacterState::Running : ECharacterState::Walking);
    }
    else
    {
        UpdateCharacterState(ECharacterState::Idle);
    }
}

void ASCUTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 设置Enhanced Input
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 移动和视角
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCUTPlayerCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCUTPlayerCharacter::Look);
        
        // 奔跑
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASCUTPlayerCharacter::StartRunning);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASCUTPlayerCharacter::StopRunning);

        // 跳跃
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASCUTPlayerCharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASCUTPlayerCharacter::StopJumping);
    }
}

void ASCUTPlayerCharacter::SetCameraDistance(float NewDistance)
{
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = NewDistance;
    }
}

void ASCUTPlayerCharacter::SetCameraRotation(const FRotator& NewRotation)
{
    if (CameraBoom)
    {
        CameraBoom->SetWorldRotation(NewRotation);
    }
}

void ASCUTPlayerCharacter::SetMovementSpeed(float NewSpeed)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
    }
}

bool ASCUTPlayerCharacter::IsMoving() const
{
    return GetVelocity().Size() > 0.0f;
}

bool ASCUTPlayerCharacter::IsRunning() const
{
    return bIsRunning;
}

void ASCUTPlayerCharacter::Move(const FInputActionValue& Value)
{
    // 添加调试日志
    UE_LOG(LogTemp, Warning, TEXT("Move Input Received: %s"), *Value.Get<FVector2D>().ToString());

    if (Controller != nullptr)
    {
        const FVector2D MovementVector = Value.Get<FVector2D>();
        
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ASCUTPlayerCharacter::Look(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D LookAxisVector = Value.Get<FVector2D>();
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ASCUTPlayerCharacter::StartRunning(const FInputActionValue& Value)
{
    bIsRunning = true;
    SetMovementSpeed(RunSpeed);
}

void ASCUTPlayerCharacter::StopRunning(const FInputActionValue& Value)
{
    bIsRunning = false;
    SetMovementSpeed(WalkSpeed);
}

void ASCUTPlayerCharacter::Jump(const FInputActionValue& Value)
{
    if (GetCharacterMovement() && GetCharacterMovement()->IsFalling() == false)
    {
        GetCharacterMovement()->JumpZVelocity = JumpHeight;
        Super::Jump();
        UpdateCharacterState(ECharacterState::Custom); // 设置为自定义状态，表示跳跃
    }
}

void ASCUTPlayerCharacter::StopJumping(const FInputActionValue& Value)
{
    Super::StopJumping();
    // 如果角色在地面上，恢复到之前的状态
    if (GetCharacterMovement() && GetCharacterMovement()->IsFalling() == false)
    {
        if (GetVelocity().Size() > 0.0f)
        {
            UpdateCharacterState(bIsRunning ? ECharacterState::Running : ECharacterState::Walking);
        }
        else
        {
            UpdateCharacterState(ECharacterState::Idle);
        }
    }
} 
