// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfigurator_GameInstance.h"

#include "DI/ObjectContainerBuilder.h"

#include "Features/Characters/Impl/DLCharacterCatalogServiceImpl.h"

//#include "Systems/MapLoad/Impl/MapLoadServiceImpl.h"
#include "Systems/ObjectTracking/Impl/DLObjectTrackerServiceImpl.h"
#include "Systems/Streamable/Impl/DLStreamableServiceImpl.h"

void FDLContainerConfigurator_GameInstance::Configure(FObjectContainerBuilder& Builder)
{
    // this method configures container for GameInstance by calling various Register methods on a Builder object
    // this is a place where you tell the container which interfaces are implemented by which classes and what lifetime of created objects will be

    // common services
    //Builder.RegisterType<UDLMapLoadServiceImpl>().As<IMapLoadService>().WeakSingleInstance();
    Builder.RegisterType<UDLCharacterCatalogServiceImpl>().As<IDLCharacterCatalogService>().SingleInstance();
    Builder.RegisterType<UDLObjectTrackerServiceImpl>().As<IDLObjectTrackerService>().SingleInstance();
    Builder.RegisterType<UDLStreamableServiceImpl>().As<IDLStreamableService>().SingleInstance();

    // configuration objects
    //Builder.RegisterDefault<UDLMapLoadServiceSettings>();
}
