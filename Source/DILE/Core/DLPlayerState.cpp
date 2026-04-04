// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLPlayerState.h"
#include "DLGameState.h"
#include "Core/DependencyManagement/DLContainerComponent.h"
#include "Core/DependencyManagement/DLContainerConfigurator_PlayerState.h"
#include "Utils/DLEnsure.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerBuilder.h"

ADLPlayerState::ADLPlayerState()
{
    ContainerComponent = CreateDefaultSubobject<UDLContainerComponent>(TEXT("ContainerComponent"));

    ContainerComponent->GetParentContainerDelegate.BindUObject(this, &ThisClass::GetParentContainer);
    ContainerComponent->ConfigureContainerDelegate.BindUObject(this, &ThisClass::ConfigureContainer);

    OnPawnSet.AddDynamic(this, &ThisClass::OnPawnChanged);

    bReplicateUsingRegisteredSubObjectList = true;
}

void ADLPlayerState::PreInitializeComponents()
{
    if (HasAuthority())
    {
        // on Authority create container as soon as possible
        ContainerComponent->CreateContainer();
    }

    Super::PreInitializeComponents();
}

UObjectContainer* ADLPlayerState::GetContainer() const
{
    return ContainerComponent->GetContainer();
}

UObjectContainer* ADLPlayerState::GetParentContainer()
{
    // Use GameState container as Parent
    UWorld* World = GetWorld();
    DL_ENSURE_RETURN(World != nullptr, nullptr);

    ADLGameState* GameState = World->GetGameState<ADLGameState>();
    DL_ENSURE_RETURN(GameState != nullptr, nullptr);

    UObjectContainer* GameStateContainer = GameState->GetContainer();
    DL_ENSURE_RETURN(GameStateContainer != nullptr, nullptr);

    return GameStateContainer;
}

void ADLPlayerState::ConfigureContainer(FObjectContainerBuilder& Builder)
{
    FDLContainerConfigurator_PlayerState::Configure(Builder, this);
}

APlayerController* ADLPlayerState::GetPlayerControllerImpl() const
{
    return Super::GetPlayerController();
}

APlayerState* ADLPlayerState::GetPlayerStateImpl() const
{
    return const_cast<ADLPlayerState*>(this);
}

APawn* ADLPlayerState::GetPawnImpl() const
{
    return Super::GetPawn();
}

void ADLPlayerState::OnPawnChanged(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
    PawnChangedDelegate.Broadcast(NewPawn);
}
