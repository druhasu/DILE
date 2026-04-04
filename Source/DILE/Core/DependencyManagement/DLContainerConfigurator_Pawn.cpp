// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfigurator_Pawn.h"
#include "DLContainerConfiguratorContext.h"
#include "Core/DLPawn.h"

#include "DI/ObjectContainerBuilder.h"

#include "Systems/Input/Impl/DLInputComponent.h"

void FDLContainerConfigurator_Pawn::Configure(FObjectContainerBuilder& Builder, ADLPawn* Pawn)
{
    // this method configures container for Pawn by calling various Register methods on a Builder object
    // this is a place where you tell the container which interfaces are implemented by which classes and what lifetime of created objects will be

    // collect some context
    FDLActorContainerConfiguratorContext Context(Pawn);

    if (Context.IsLocal())
    {
        Builder.RegisterInstance(Cast<UDLInputComponent>(Pawn->GetController()->InputComponent)).As<IDLInputBindingService>();
    }
}
