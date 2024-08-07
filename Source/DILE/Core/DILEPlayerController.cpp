// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DILEPlayerController.h"
#include "DI/InjectOnConstruction.h"
#include "DI/ObjectContainerBuilder.h"
#include "DI/ObjectContainer.h"
#include "Systems/ServiceReplication/ServiceReplicatorComponent.h"

ADILEPlayerController::ADILEPlayerController()
{
    ServiceReplicator = CreateDefaultSubobject<UServiceReplicatorComponent>(TEXT("ServiceReplicator"));
}

void ADILEPlayerController::PreInitializeComponents()
{
    // We want to create container as soon as possible
    // We do it before components are initialized in case some of them use FInjectOnConstruction::TryInitDependencies
    // In multiplayer games this is also the earliest possible moment to create container
    CreateContainer();

    Super::PreInitializeComponents();
}

void ADILEPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    PlayerStateChangedDelegate.Broadcast(PlayerState.Get());
}

void ADILEPlayerController::SetPawn(APawn* InPawn)
{
    // We use SetPawn instead of OnRep_PlayerPawn because latter may be not called due to order of replication
    // Our pawn may be changed from APawn::OnRep_Controller without triggering OnRep callback
    Super::SetPawn(InPawn);
    PawnChangedDelegate.Broadcast(InPawn);
}

void ADILEPlayerController::CreateContainer()
{
    if (!ensure(Container == nullptr))
    {
        // should not be called more than once
        return;
    }

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr) || !(World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE))
    {
        // we are not in Game world (probably Editor or Preview instance)
        return;
    }

    // Use current world container as Parent
    UObjectContainer* WorldContainer = FInjectOnConstruction::GetContainerForWorld(World);
    if (ensure(WorldContainer != nullptr))
    {
        FObjectContainerBuilder Builder;

        // Set this instance as Outer for all new objects created by Container
        // We need this for proper service replication
        Builder.SetOuterForNewObjects(this);

        // Register ourself as Player classes provider
        Builder.RegisterInstance(this)
            .As<IPlayerControllerService>()
            .As<IPlayerStateService>()
            .As<IPlayerPawnService>();

        // TODO: Register additional services here

        Container = Builder.BuildNested(*WorldContainer);

        // Inject dependencies into ServiceReplicator
        Container->Inject(ServiceReplicator);

        // It may be useful to see in logs when container was created
        UE_LOG(LogTemp, Log, TEXT("ObjectContainer created for Player"));
    }
}
