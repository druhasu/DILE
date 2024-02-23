// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Core/DILEGameInstance.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerBuilder.h"
#include "DI/InjectOnConstruction.h"

void UDILEGameInstance::Init()
{
    // create global game container before calling base Init.
    // Init will create Subsystems and they may need Container
    CreateContainer();

    // call Super to initialize Subsystems
    Super::Init();

    // process current World if it is already created
    // initialization process differs between packaged game and PIE, this call helps us cover both of them
    OnWorldChanged(nullptr, GetWorld());
}

void UDILEGameInstance::Shutdown()
{
    Super::Shutdown();

    // process current World if it still exists
    // we need this call to clear its Container association
    OnWorldChanged(GetWorld(), nullptr);
}

void UDILEGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
    if (Container == nullptr)
    {
        // in PIE we may receive this call before Init. just ignore in this case
        return;
    }

    if (NewWorld == nullptr)
    {
        // we are unloading previous level, make sure no container is associated with it
        if (OldWorld != nullptr)
        {
            FInjectOnConstruction::ClearContainerForWorld(OldWorld);
        }

        return;
    }

    // associate our Container with new World
    // this will allow us to use FInjectOnConstruction::TryInitDependencies from objects that were not created by the container
    FInjectOnConstruction::SetContainerForWorld(NewWorld, Container);
}

void UDILEGameInstance::CreateContainer()
{
    FObjectContainerBuilder Builder;
    ConfigureContainer(Builder);

    Container = Builder.Build(this);
}
