// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "MoverComponent.h"
#include "Misc/Optional.h"

#include "DLMoverComponent.generated.h"

class IDLPlayerControllerService;

UCLASS()
class DILE_API UDLMoverComponent : public UMoverComponent
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TOptional<TScriptInterface<IDLPlayerControllerService>> InPlayerControllerService
    );

    /* Whether we are falling. Helper for access in threadsafe animation update */
    UFUNCTION(BlueprintCallable)
    bool IsFalling() const;

    /* Whether we are on the ground. Helper for access in threadsafe animation update */
    UFUNCTION(BlueprintCallable)
    bool IsOnGround() const;

private:
    UFUNCTION()
    void OnFrameFinalized(const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState);

private:
    UPROPERTY()
    TScriptInterface<IDLPlayerControllerService> PlayerControllerService;
};
