// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLGameInstance.h"
#include "Core/DependencyManagement/DLContainerConfigurator_GameInstance.h"
#include "Core/DependencyManagement/DLContainerConfiguratorContext.h"
#include "Utils/DLEnsure.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerBuilder.h"

void UDLGameInstance::Init()
{
    Super::Init();

    InitContainer();
}

void UDLGameInstance::Shutdown()
{
    StartupShutdownHelper.Shutdown();
    Super::Shutdown();
}

void UDLGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
    Super::OnWorldChanged(OldWorld, NewWorld);
}

void UDLGameInstance::InitContainer()
{
    DL_ENSURE_RETURN(Container == nullptr);

    FObjectContainerBuilder Builder;
    Builder.SetOuterForNewObjects(this);

    FDLContainerConfigurator_GameInstance::Configure(Builder);

    Container = Builder.Build(this);

    StartupShutdownHelper.Startup(Container);
}
