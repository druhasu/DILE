// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLPawn.h"
#include "DLPlayerState.h"
#include "Core/DependencyManagement/DLContainerComponent.h"
#include "Core/DependencyManagement/DLContainerConfigurator_Pawn.h"
#include "Utils/DLEnsure.h"

#include "AbilitySystemComponent.h"
#include "DI/ObjectContainer.h"

ADLPawn::ADLPawn()
{
    SetReplicatingMovement(false);	// disable Actor-level movement replication, since our Mover component will handle it

    ContainerComponent = CreateDefaultSubobject<UDLContainerComponent>(TEXT("ContainerComponent"));

    ContainerComponent->GetParentContainerDelegate.BindUObject(this, &ThisClass::GetParentContainer);
    ContainerComponent->ConfigureContainerDelegate.BindUObject(this, &ThisClass::ConfigureContainer);

    bReplicateUsingRegisteredSubObjectList = true;
}

void ADLPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // on Authority create container as soon as possible
    ContainerComponent->CreateContainer();
}

UAbilitySystemComponent* ADLPawn::GetAbilitySystemComponent() const
{
    // We must implement IAbilitySystemInterface here because our ASC may live on the PlayerState
    // There is no other way to link that ASC with the Pawn

    return ContainerComponent->GetContainer()->TryResolve<UAbilitySystemComponent>();
}

UObjectContainer* ADLPawn::GetParentContainer()
{
    // Use PlayerState container as Parent
    ADLPlayerState* CastedPlayerState = GetPlayerState<ADLPlayerState>();
    DL_ENSURE_RETURN(CastedPlayerState != nullptr, nullptr);

    UObjectContainer* PlayerStateContainer = CastedPlayerState->GetContainer();
    DL_ENSURE_RETURN(PlayerStateContainer != nullptr, nullptr);

    return PlayerStateContainer;
}

void ADLPawn::ConfigureContainer(FObjectContainerBuilder& Builder)
{
    FDLContainerConfigurator_Pawn::Configure(Builder, this);
}
