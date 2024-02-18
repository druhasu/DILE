// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Core/DILEGameInstance.h"
#include "DI/ObjectContainerBuilder.h"

#include "Services/MapLoadServiceImpl.h"

void UDILEGameInstance::ConfigureContainer(FObjectContainerBuilder& Builder)
{
    // this method configures container by calling various Register methods on a Builder object
    // this is a place where you tell the container which interfaces are implemented by which classes and what lifetime of created objects will be

    // common services
    Builder.RegisterType<UMapLoadServiceImpl>().As<IMapLoadService>().WeakSingleInstance();

    // configuration objects
    Builder.RegisterDefault<UMapLoadServiceSettings>();
}
