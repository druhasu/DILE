// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfigurator_PlayerState.h"
#include "DLContainerConfiguratorContext.h"
#include "Core/DLPlayerState.h"

#include "DI/ObjectContainerBuilder.h"

#include "Features/CharacterSelection/Impl/DLCharacterSelectionServiceImpl.h"

#include "Systems/ViewFactory/Impl/DLViewFactoryImpl.h"
#include "Systems/ViewModelFactory/Impl/DLViewModelFactoryImpl.h"
#include "Systems/WidgetDisplay/Impl/DLViewportWidgetServiceImpl.h"
#include "Systems/WidgetDisplay/Impl/DLWidgetDisplayServiceImpl.h"
#include "Systems/WidgetSlots/Impl/DLWidgetSlotsServiceImpl.h"

void FDLContainerConfigurator_PlayerState::Configure(FObjectContainerBuilder& Builder, ADLPlayerState* PlayerState)
{
    // this method configures container for Player by calling various Register methods on a Builder object
    // this is a place where you tell the container which interfaces are implemented by which classes and what lifetime of created objects will be

    // collect some context
    FDLActorContainerConfiguratorContext Context(PlayerState);

    if (Context.IsLocal())
    {
        Builder.RegisterType<UDLViewFactoryImpl>().As<IDLViewFactory>().SingleInstance();
        Builder.RegisterType<UDLViewModelFactoryImpl>().As<IDLViewModelFactory>().SingleInstance();
        Builder.RegisterType<UDLViewportWidgetServiceImpl>().As<IDLViewportWidgetService>().As<IDLShutdownListener>().SingleInstance();
        Builder.RegisterType<UDLWidgetDisplayServiceImpl>().As<IDLWidgetDisplayService>().SingleInstance(true);
        Builder.RegisterType<UDLWidgetSlotsServiceImpl>().As<IDLWidgetSlotsService>().As<IDLShutdownListener>().SingleInstance();
    }

    if (Context.HasServerLogic() || Context.IsLocal())
    {
        Builder.RegisterType<UDLCharacterSelectionServiceImpl>().As<IDLCharacterSelectionService>().As<IDLReplicatedService>().As<IDLStartupListener>().SingleInstance(true);
    }

    Builder.RegisterInstance(PlayerState)
        .As<IDLPlayerControllerService>()
        .As<IDLPlayerStateService>()
        .As<IDLPlayerPawnService>();
}
