// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Systems/PlayerContext/IPlayerControllerService.h"
#include "Systems/PlayerContext/IPlayerPawnService.h"
#include "Systems/PlayerContext/IPlayerStateService.h"
#include "DILEPlayerController.generated.h"

class UObjectContainer;
class UServiceReplicatorComponent;

/*
 * Player controller class for main Game mode
 */
UCLASS()
class ADILEPlayerController : public APlayerController, public IPlayerControllerService, public IPlayerPawnService, public IPlayerStateService
{
    GENERATED_BODY()

public:
    ADILEPlayerController();

    void PreInitializeComponents() override;

    FPawnChangedSignature& OnPawnChanged() override { return PawnChangedDelegate; };
    FPlayerStateChangedSignature& OnPlayerStateChanged() override { return PlayerStateChangedDelegate; };

    void OnRep_PlayerState() override;
    void SetPawn(APawn* InPawn) override;

private:
    void CreateContainer();

    APlayerController* GetPlayerControllerImpl() const override { return const_cast<ADILEPlayerController*>(this); }
    APawn* GetPawnImpl() const override { return Super::GetPawn(); }
    APlayerState* GetPlayerStateImpl() const override { return PlayerState.Get(); }

    /* Container of this Player */
    UPROPERTY()
    TObjectPtr<UObjectContainer> Container;

    /* Component for Services replication */
    UPROPERTY()
    TObjectPtr<UServiceReplicatorComponent> ServiceReplicator;

    FPawnChangedSignature PawnChangedDelegate;
    FPlayerStateChangedSignature PlayerStateChangedDelegate;
};
