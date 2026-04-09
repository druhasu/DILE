// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLGameState.h"
#include "DLGameInstance.h"
#include "Core/DependencyManagement/DLContainerComponent.h"
#include "Core/DependencyManagement/DLContainerConfigurator_GameState.h"
#include "Utils/DLEnsure.h"

#include "DI/ObjectContainerBuilder.h"
#include "DI/InjectOnConstruction.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

ADLGameState::ADLGameState()
{
    ContainerComponent = CreateDefaultSubobject<UDLContainerComponent>(TEXT("ContainerComponent"));

    ContainerComponent->GetParentContainerDelegate.BindUObject(this, &ThisClass::GetParentContainer);
    ContainerComponent->ConfigureContainerDelegate.BindUObject(this, &ThisClass::ConfigureContainer);
    ContainerComponent->ContainerCreatedDelegate.BindUObject(this, &ThisClass::OnContainerCreated);

    bReplicateUsingRegisteredSubObjectList = true;
}

void ADLGameState::ReceivedGameModeClass()
{
    if (HasAuthority())
    {
        ContainerComponent->CreateContainer();
    }

    Super::ReceivedGameModeClass();
}

void ADLGameState::EndPlay(EEndPlayReason::Type InReason)
{
    Super::EndPlay(InReason);
    FInjectOnConstruction::ClearContainerForWorld(GetWorld());
}

UObjectContainer* ADLGameState::GetContainer() const
{
    return ContainerComponent->GetContainer();
}

void ADLGameState::OnRep_ReplicatedHasBegunPlay()
{
    if (bReplicatedHasBegunPlay && GetLocalRole() != ROLE_Authority)
    {
        GetWorld()->GetTimerManager().SetTimer(BeginPlayTimer, this, &ThisClass::TryDispatchBeginPlay, 0.01f, true);
    }
}

UObjectContainer* ADLGameState::GetParentContainer()
{
    // Use GameInstance container as Parent
    UWorld* World = GetWorld();
    DL_ENSURE_RETURN(World != nullptr, nullptr);

    UDLGameInstance* GameInstance = World->GetGameInstance<UDLGameInstance>();
    DL_ENSURE_RETURN(GameInstance != nullptr, nullptr);

    UObjectContainer* GameInstanceContainer = GameInstance->GetContainer();
    DL_ENSURE_RETURN(GameInstanceContainer != nullptr, nullptr);

    return GameInstanceContainer;
}

void ADLGameState::ConfigureContainer(FObjectContainerBuilder& Builder)
{
    FDLContainerConfigurator_GameState::Configure(Builder, this);
}

void ADLGameState::OnContainerCreated(UObjectContainer* Container)
{
    FInjectOnConstruction::SetContainerForWorld(GetWorld(), Container);
}

void ADLGameState::TryDispatchBeginPlay()
{
    if (IsReadyForBeginPlay())
    {
        Super::OnRep_ReplicatedHasBegunPlay();
        GetWorld()->GetTimerManager().ClearTimer(BeginPlayTimer);
    }
}

bool ADLGameState::IsReadyForBeginPlay() const
{
#define TryCondition(X) \
    if (!(X)) { return false; }

    UWorld* World = GetWorld();
    APlayerController* MyController = World->GetFirstPlayerController();
    TryCondition(MyController != nullptr);
    TryCondition(MyController->GetHUD() != nullptr);
    TryCondition(MyController->Player != nullptr);

    APlayerState* MyPlayerState = MyController->GetPlayerState<APlayerState>();
    TryCondition(MyPlayerState != nullptr);

#undef TryCondition

    return true;
}
