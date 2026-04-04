// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInputProducer.h"
#include "Systems/PlayerContext/IDLPlayerControllerService.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"
#include "Systems/Input/IDLInputBindingService.h"

#include "MoverComponent.h"
#include "InputAction.h"
#include "InputTriggers.h"

void UDLInputProducer::InitDependencies(
    UMoverComponent* InMoverComponent,
    TScriptInterface<IDLPlayerControllerService> InPlayerControllerService,
    TScriptInterface<IDLPlayerPawnService> InPawnService,
    TScriptInterface<IDLInputBindingService> InInputBindingService
)
{
    InMoverComponent->InputProducers.Add(this);
    PlayerControllerService = InPlayerControllerService;
    PawnService = InPawnService;
    InputBindingService = InInputBindingService;

    using FDlg = IDLInputBindingService::FActionEventSignature;
    InputBindingService->BindInput(MoveInputAction, ETriggerEvent::Triggered, FDlg::CreateUObject(this, &ThisClass::OnMoveTriggered));
    InputBindingService->BindInput(MoveInputAction, ETriggerEvent::Completed, FDlg::CreateUObject(this, &ThisClass::OnMoveCompleted));
    InputBindingService->BindInput(LookInputAction, ETriggerEvent::Triggered, FDlg::CreateUObject(this, &ThisClass::OnLookTriggered));
    InputBindingService->BindInput(LookInputAction, ETriggerEvent::Completed, FDlg::CreateUObject(this, &ThisClass::OnLookCompleted));
    InputBindingService->BindInput(JumpInputAction, ETriggerEvent::Started,   FDlg::CreateUObject(this, &ThisClass::OnJumpStarted));
    InputBindingService->BindInput(JumpInputAction, ETriggerEvent::Completed, FDlg::CreateUObject(this, &ThisClass::OnJumpReleased));
}

void UDLInputProducer::OnShutdown()
{
    InputBindingService->UnbindInput(this);
}

void UDLInputProducer::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& OutInputCmd)
{
    FCharacterDefaultInputs& CharacterInputs = OutInputCmd.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

    CharacterInputs.ControlRotation = FRotator::ZeroRotator;
    if (APawn* Pawn = PawnService->GetPawn())
    {
        CharacterInputs.ControlRotation = Pawn->GetControlRotation();
    }

    const FVector FinalDirectionalIntent = CharacterInputs.ControlRotation.RotateVector(CachedMoveInputIntent);
    CharacterInputs.SetMoveInput(EMoveInputType::DirectionalIntent, FinalDirectionalIntent);

    // Figure out intended orientation
    CharacterInputs.OrientationIntent = FVector::ZeroVector;

    const bool bHasAffirmativeMoveInput = (CharacterInputs.GetMoveInput().SizeSquared() >= KINDA_SMALL_NUMBER);
    if (bHasAffirmativeMoveInput)
    {
        // set intent to the the control rotation - often a player's camera rotation
        LastAffirmativeMoveInput = CharacterInputs.GetMoveInput();
    }

    CharacterInputs.OrientationIntent = CharacterInputs.ControlRotation.Vector().GetSafeNormal();

    CharacterInputs.bIsJumpPressed = bIsJumpPressed;
    CharacterInputs.bIsJumpJustPressed = bIsJumpJustPressed;

    // Convert inputs to be relative to the current movement base (depending on options and state)
    CharacterInputs.bUsingMovementBase = false;

    // Clear/consume temporal movement inputs. We are not consuming others in the event that the game world is ticking at a lower rate than the Mover simulation. 
    // In that case, we want most input to carry over between simulation frames.
    bIsJumpJustPressed = false;
}

void UDLInputProducer::OnMoveTriggered(const FInputActionInstance& Value)
{
    const FVector MovementVector = Value.GetValue().Get<FVector>();
    CachedMoveInputIntent.X = FMath::Clamp(MovementVector.Y, -1.0f, 1.0f);
    CachedMoveInputIntent.Y = FMath::Clamp(MovementVector.X, -1.0f, 1.0f);
    CachedMoveInputIntent.Z = FMath::Clamp(MovementVector.Z, -1.0f, 1.0f);
}

void UDLInputProducer::OnMoveCompleted(const FInputActionInstance& Value)
{
    CachedMoveInputIntent = FVector::ZeroVector;
}

void UDLInputProducer::OnLookTriggered(const FInputActionInstance& Value)
{
    const FVector2D LookVector = Value.GetValue().Get<FVector2D>();
    CachedLookInput.Yaw = CachedTurnInput.Yaw = LookVector.X;
    CachedLookInput.Pitch = CachedTurnInput.Pitch = LookVector.Y;

    // Simple input scaling. A real game will probably map this to some kind of sensitivity settings
    static constexpr float LookRateYaw = 1.f;
    static constexpr float LookRatePitch = 1.f;

    APlayerController* PC = PlayerControllerService->GetPlayerController();
    PC->AddYawInput(CachedLookInput.Yaw * LookRateYaw);
    PC->AddPitchInput(-CachedLookInput.Pitch * LookRatePitch);
}

void UDLInputProducer::OnLookCompleted(const FInputActionInstance& Value)
{
    CachedLookInput = FRotator::ZeroRotator;
    CachedTurnInput = FRotator::ZeroRotator;
}

void UDLInputProducer::OnJumpStarted(const FInputActionInstance& Value)
{
    bIsJumpJustPressed = !bIsJumpPressed;
    bIsJumpPressed = true;
}

void UDLInputProducer::OnJumpReleased(const FInputActionInstance& Value)
{
    bIsJumpPressed = false;
    bIsJumpJustPressed = false;
}
