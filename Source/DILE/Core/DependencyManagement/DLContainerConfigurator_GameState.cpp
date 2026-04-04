// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfigurator_GameState.h"
#include "Core/DLGameState.h"
#include "Core/DLGameMode.h"

#include "DI/ObjectContainerBuilder.h"

#include "Core/DependencyManagement/DLInjectorProvider.h"

#include "Features/Respawn/Impl/DLRespawnServiceImpl.h"

#include "Systems/GameModeEvents/IDLGameModeEventsService.h"
#include "Systems/Timers/Impl/DLTimerServiceImpl.h"

void FDLContainerConfigurator_GameState::Configure(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context)
{
    // this method configures container for GameState by calling various Register methods on a Builder object
    // this is a place where you tell the container which interfaces are implemented by which classes and what lifetime of created objects will be

    ADLGameState* GameState = Cast<ADLGameState>(Context.GetActor());
    ADLGameMode* GameMode = GameState->GetWorld()->GetAuthGameMode<ADLGameMode>();

    Builder.RegisterType<UDLInjectorProvider>().As<IInjectorProvider>().SingleInstance(true);
    Builder.RegisterType<UDLTimerServiceImpl>().As<IDLTimerService>().SingleInstance();

    if (Context.HasServerLogic())
    {
        Builder.RegisterType<UDLRespawnServiceImpl>().As<IDLRespawnService>().SingleInstance(true);
        Builder.RegisterInstance(GameMode).As<IDLGameModeEventsService>();
    }
}
