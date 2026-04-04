// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "MoverSimulationTypes.h"
#include "Systems/Lifecycle/IDLShutdownListener.h"

#include "DLInputProducer.generated.h"

class UMoverComponent;
class UInputAction;
class IDLPlayerControllerService;
class IDLPlayerPawnService;
class IDLInputBindingService;
struct FInputActionInstance;

UCLASS(Blueprintable)
class UDLInputProducer : public UObject, public IMoverInputProducerInterface, public IDLShutdownListener
{
    GENERATED_BODY()

public:
    void InitDependencies(
        UMoverComponent* InMoverComponent,
        TScriptInterface<IDLPlayerControllerService> InPlayerControllerService,
        TScriptInterface<IDLPlayerPawnService> InPlayerPawnService,
        TScriptInterface<IDLInputBindingService> InInputBindingService
    );

    void OnShutdown() override;

    void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& OutInputCmd) override;

protected:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UInputAction> MoveInputAction;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UInputAction> LookInputAction;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UInputAction> JumpInputAction;

private:
    void OnMoveTriggered(const FInputActionInstance& Value);
    void OnMoveCompleted(const FInputActionInstance& Value);
    void OnLookTriggered(const FInputActionInstance& Value);
    void OnLookCompleted(const FInputActionInstance& Value);
    void OnJumpStarted(const FInputActionInstance& Value);
    void OnJumpReleased(const FInputActionInstance& Value);

private:
    UPROPERTY()
    TScriptInterface<IDLPlayerControllerService> PlayerControllerService;

    UPROPERTY()
    TScriptInterface<IDLPlayerPawnService> PawnService;

    UPROPERTY()
    TScriptInterface<IDLInputBindingService> InputBindingService;

    FVector LastAffirmativeMoveInput = FVector::ZeroVector;	// Movement input (intent or velocity) the last time we had one that wasn't zero

    FVector CachedMoveInputIntent = FVector::ZeroVector;

    FRotator CachedTurnInput = FRotator::ZeroRotator;
    FRotator CachedLookInput = FRotator::ZeroRotator;

    bool bIsJumpJustPressed = false;
    bool bIsJumpPressed = false;
};
