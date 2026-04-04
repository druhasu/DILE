// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Systems/Lifecycle/Impl/DLStartupShutdownHelper.h"
#include "Systems/ServiceReplication/IDLSubObjectReplicationService.h"

#include "DI/IResolver.h"

#include "DLContainerComponent.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;

UCLASS()
class UDLContainerComponent : public UActorComponent, public IResolver, public IDLSubObjectReplicationService
{
    GENERATED_BODY()

public:
    UDLContainerComponent();

    // Begin IResolver
    UObject* Resolve(UClass* Type) const override;
    TObjectsCollection<UObject> ResolveAll(UClass* Type) const override;
    TFactory<UObject> ResolveFactory(UClass* Type) const override;
    UObject* TryResolve(UClass* Type) const override;
    TObjectsCollection<UObject> TryResolveAll(UClass* Type) const override;
    TFactory<UObject> TryResolveFactory(UClass* Type) const override;
    bool IsRegistered(UClass* Type) const override;
    // End IResolver

    // Begin IDLSubObjectReplicationService
    UObject* GetOuterForSubObjects() const override { return GetOwner(); }
    void AddSubObject(UObject* SubObject, ELifetimeCondition NetCondition) override;
    void RemoveSubObject(UObject* SubObject) override;
    // End IDLSubObjectReplicationService

    void PreNetReceive() override;
    void BeginPlay() override;
    void EndPlay(EEndPlayReason::Type InReason) override;

    void CreateContainer();

    UObjectContainer* GetContainer() const { return Container; }

public:
    TDelegate<UObjectContainer* ()> GetParentContainerDelegate;
    TDelegate<void (FObjectContainerBuilder& Builder)> ConfigureContainerDelegate;
    TDelegate<void (UObjectContainer* NewContainer)> ContainerCreatedDelegate;

private:
    UPROPERTY()
    TObjectPtr<UObjectContainer> Container;

    UPROPERTY()
    TArray<TObjectPtr<UObject>> ReplicatedServices;

    UPROPERTY()
    FDLStartupShutdownHelper StartupShutdownHelper;

    UPROPERTY(Replicated)
    bool bContainerCreated = false;
};
