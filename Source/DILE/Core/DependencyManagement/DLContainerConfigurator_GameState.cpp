// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfigurator_GameState.h"
#include "DLContainerConfiguratorContext.h"
#include "DLInjectorProvider.h"
#include "DLComponentInstanceFactory.h"
#include "Core/DLGameState.h"
#include "Core/DLGameMode.h"
#include "Systems/GameModeEvents/IDLGameModeEventsService.h"
#include "Systems/NetMode/Impl/DLNetModeServiceImpl.h"
#include "Systems/Timers/Impl/DLTimerServiceImpl.h"

#include "DI/ObjectContainerBuilder.h"

void FDLContainerConfigurator_GameState::Configure(FObjectContainerBuilder& Builder, ADLGameState* GameState)
{
    // this method configures container for GameState by calling various Register methods on a Builder object
    // this is a place where you tell the container which interfaces are implemented by which classes and what lifetime of created objects will be

    // collect some context
    FDLActorContainerConfiguratorContext Context(GameState);

    Builder.RegisterType<UDLInjectorProvider>().As<IInjectorProvider>().SingleInstance(true);
    Builder.RegisterType<UDLNetModeServiceImpl>().As<IDLNetModeService>().SingleInstance();
    Builder.RegisterType<UDLTimerServiceImpl>().As<IDLTimerService>().SingleInstance();

    // Register InstanceFactory for components
    Builder.RegisterDefault<UDLComponentInstanceFactory>().As<IInstanceFactory>();

    if (Context.HasServerLogic())
    {
        ADLGameMode* GameMode = GameState->GetWorld()->GetAuthGameMode<ADLGameMode>();
        Builder.RegisterInstance(GameMode).As<IDLGameModeEventsService>();
    }
}
