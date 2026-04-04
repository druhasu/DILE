// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLContainerConfiguratorContext.h"

class FObjectContainerBuilder;
class FDLActorContainerConfiguratorContext;

struct FDLContainerConfigurator_GameState
{
    static void Configure(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context);
};
