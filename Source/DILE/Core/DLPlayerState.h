// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"

#include "Systems/PlayerContext/IDLPlayerControllerService.h"
#include "Systems/PlayerContext/IDLPlayerStateService.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"

#include "DLPlayerState.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;
class UDLContainerComponent;

UCLASS()
class DILE_API ADLPlayerState
    : public APlayerState
    , public IDLPlayerControllerService
    , public IDLPlayerStateService
    , public IDLPlayerPawnService
{
    GENERATED_BODY()

public:
    ADLPlayerState();

    void PreInitializeComponents() override;

    UObjectContainer* GetContainer() const;

private:
    UObjectContainer* GetParentContainer();
    void ConfigureContainer(FObjectContainerBuilder& Builder);

    // Begin IDLPlayerControllerService
    APlayerController* GetPlayerControllerImpl() const override;
    // End IDLPlayerControllerService

    // Begin IDLPlayerStateService
    APlayerState* GetPlayerStateImpl() const override;
    // End IDLPlayerStateService

    // Begin IDLPlayerPawnService
    APawn* GetPawnImpl() const override;
    FPawnChangedSignature& OnPawnChanged() override { return PawnChangedDelegate; }
    // End IDLPlayerPawnService

    UFUNCTION()
    void OnPawnChanged(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);

private:
    UPROPERTY(VisibleInstanceOnly)
    TObjectPtr<UDLContainerComponent> ContainerComponent;

    FPawnChangedSignature PawnChangedDelegate;

};
