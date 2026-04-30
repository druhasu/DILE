// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerComponent.h"
#include "Core/DLGameMode.h"
#include "Core/DLGameState.h"
#include "Core/DLGameModeConfigurationAsset.h"
#include "Core/DependencyManagement/DLContainerConfiguratorContext.h"
#include "Systems/ServiceReplication/DLReplicatedService.h"
#include "Utils/DLEnsure.h"
#include "Utils/DLLog.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerBuilder.h"
#include "DI/ObjectContainerIterator.h"

UDLContainerComponent::UDLContainerComponent()
{
    SetIsReplicatedByDefault(true);
    PrimaryComponentTick.bCanEverTick = false;

    bReplicateUsingRegisteredSubObjectList = true;
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

void UDLContainerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Container != nullptr)
    {
        // call startup logic if Container is alredy created
        StartupShutdownHelper.Startup(Container);
    }
}

void UDLContainerComponent::EndPlay(EEndPlayReason::Type InReason)
{
    for (UObject* Service : ReplicatedServices)
    {
        RemoveReplicatedSubObject(Service);
    }

    if (InitHelper != nullptr)
        GetOwner()->RemoveReplicatedSubObject(InitHelper);

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

    // if we are a Server instance, create InitHelper and add it to replication
    // we need this for proper initialization of Container on clients
    ENetMode NetMode = World->GetNetMode();
    if (NetMode == NM_ListenServer || NetMode == NM_DedicatedServer)
    {
        InitHelper = NewObject<UDLContainerComponentInitHelper>(GetOwner());

        // add to Owner's SubObject list, not to ours
        // Actor SubObjects are always replicated before the SubObjects of its components
        // we use this rule to make sure Container on clients is created before replicated services are processed
        GetOwner()->AddReplicatedSubObject(InitHelper, COND_InitialOnly);
    }

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

    // remove itself from further processing
    InitialComponents.Remove(this);

    // register initial components by interfaces
    for (UActorComponent* Component : InitialComponents)
    {
        if (Component->GetClass()->Interfaces.Num() > 0)
        {
            Builder.RegisterInstance(Component).ByInterfaces();
        }
    }

    Container = Builder.BuildNested(*ParentContainer);

    // Inject dependencies into initial components
    for (UActorComponent* Component : InitialComponents)
    {
        Container->Inject(Component);
    }

    ContainerCreatedDelegate.ExecuteIfBound(Container);

    // Collect and process all replicated services
    for (auto It = Container->CreateIterator<IDLReplicatedService>(); It; ++It)
    {
        TScriptInterface<IDLReplicatedService> Service = It.Get(true);

        AddReplicatedSubObject(Service.GetObject(), Service->GetReplicationCondition());
        ReplicatedServices.Add(Service.GetObject());
    }

    if (HasBegunPlay())
    {
        // call startup logic right away if BeginPlay has already happened
        StartupShutdownHelper.Startup(Container);
    }

    // It may be useful to see in logs when container was created
    DL_LOG(Log, TEXT("ObjectContainer created for %s"), *GetOwner()->GetName());
}

void UDLContainerComponentInitHelper::OnCreatedFromReplication()
{
    AActor* Actor = GetTypedOuter<AActor>();
    DL_ENSURE_RETURN(Actor != nullptr);

    UDLContainerComponent* ContainerComponent = Actor->FindComponentByClass<UDLContainerComponent>();
    DL_ENSURE_RETURN(ContainerComponent != nullptr);
    DL_ENSURE_RETURN(ContainerComponent->GetContainer() == nullptr);

    ContainerComponent->CreateContainer();
}
