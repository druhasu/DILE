// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerComponent.h"
#include "Core/DLGameMode.h"
#include "Core/DLGameState.h"
#include "Core/DLContainerConfigurationAsset.h"
#include "Core/DependencyManagement/DLContainerConfiguratorContext.h"
#include "Systems/ServiceReplication/DLReplicatedService.h"
#include "Utils/DLEnsure.h"
#include "Utils/DLLog.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerBuilder.h"
#include "DI/ObjectContainerIterator.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Net/UnrealNetwork.h"

UDLContainerComponent::UDLContainerComponent()
{
    SetIsReplicatedByDefault(true);

    bReplicateUsingRegisteredSubObjectList = true;
}

void UDLContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, bContainerCreated);
}

UObject* UDLContainerComponent::Resolve(UClass* Type) const
{
    return Container->Resolve(Type);
}

TObjectsCollection<UObject> UDLContainerComponent::ResolveAll(UClass* Type) const
{
    return Container->ResolveAll(Type);
}

TFactory<UObject> UDLContainerComponent::ResolveFactory(UClass* Type) const
{
    return Container->ResolveFactory(Type);
}

UObject* UDLContainerComponent::TryResolve(UClass* Type) const
{
    return Container->TryResolve(Type);
}

TObjectsCollection<UObject> UDLContainerComponent::TryResolveAll(UClass* Type) const
{
    return Container->TryResolveAll(Type);
}

TFactory<UObject> UDLContainerComponent::TryResolveFactory(UClass* Type) const
{
    return Container->TryResolveFactory(Type);
}

bool UDLContainerComponent::IsRegistered(UClass* Type) const
{
    return Container->IsRegistered(Type);
}

void UDLContainerComponent::AddSubObject(UObject* SubObject, ELifetimeCondition NetCondition)
{
    AddReplicatedSubObject(SubObject, NetCondition);
    GetOwner()->ForceNetUpdate();
}

void UDLContainerComponent::RemoveSubObject(UObject* SubObject)
{
    RemoveReplicatedSubObject(SubObject);
    GetOwner()->ForceNetUpdate();
}

void UDLContainerComponent::PreNetReceive()
{
    if (Container == nullptr)
    {
        CreateContainer();
    }

    Super::PreNetReceive();
}

void UDLContainerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Container != nullptr)
    {
        StartupShutdownHelper.Startup(Container);
    }
}

void UDLContainerComponent::EndPlay(EEndPlayReason::Type InReason)
{
    for (UObject* Service : ReplicatedServices)
    {
        RemoveReplicatedSubObject(Service);
    }

    StartupShutdownHelper.Shutdown();

    Super::EndPlay(InReason);
}

void UDLContainerComponent::CreateContainer()
{
    // should not be called more than once
    DL_ENSURE_RETURN(Container == nullptr);

    UWorld* World = GetWorld();
    DL_ENSURE_RETURN(World != nullptr);

    if (!(World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE))
    {
        // we are not in Game world (probably Editor or Preview instance)
        return;
    }

    ADLGameState* GameState = World->GetGameState<ADLGameState>();
    DL_ENSURE_RETURN(GameState != nullptr);

    const ADLGameMode* GameMode = GameState->GetDefaultGameMode<ADLGameMode>();
    DL_ENSURE_RETURN(GameMode != nullptr);

    const UDLGameModeConfigurationAsset* Configuration = GameMode->GetConfiguration();
    DL_ENSURE_RETURN(Configuration != nullptr);

    DL_ENSURE_RETURN(GetParentContainerDelegate.IsBound());
    DL_ENSURE_RETURN(ConfigureContainerDelegate.IsBound());

    UObjectContainer* ParentContainer = GetParentContainerDelegate.Execute();
    DL_ENSURE_RETURN(ParentContainer != nullptr);

    FObjectContainerBuilder Builder;

    FDLActorContainerConfiguratorContext Context(GetOwner());
    Configuration->RegisterClassesForActor(Builder, Context);

    // Set this instance as Outer for all new objects created by Container
    // We need this for proper service replication
    Builder.SetOuterForNewObjects(GetOwner());

    // register self as IDLSubObjectReplicationService to allow other services replicate their SubObjects
    Builder.RegisterInstance(this).As<IDLSubObjectReplicationService>();

    ConfigureContainerDelegate.Execute(Builder);

    // Remember which components existed before container creation
    TSet<UActorComponent*, DefaultKeyFuncs<UActorComponent*>, TInlineSetAllocator<16>> InitialComponents(GetOwner()->GetComponents());

    Container = Builder.BuildNested(*ParentContainer);
    ContainerCreatedDelegate.ExecuteIfBound(Container);

    // Inject dependencies into initial components
    for (UActorComponent* Component : InitialComponents)
    {
        Container->Inject(Component);
    }

    // Collect and process all replicated services
    for (auto It = Container->CreateIterator<IDLReplicatedService>(); It; ++It)
    {
        TScriptInterface<IDLReplicatedService> Service = It.Get(true);

        AddReplicatedSubObject(Service.GetObject(), Service->GetReplicationCondition());
        ReplicatedServices.Add(Service.GetObject());
    }

    if (HasBegunPlay())
    {
        StartupShutdownHelper.Startup(Container);
    }

    MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, bContainerCreated, this);
    bContainerCreated = true;

    // It may be useful to see in logs when container was created
    DL_LOG(Log, TEXT("ObjectContainer created for %s"), *GetOwner()->GetName());
}
