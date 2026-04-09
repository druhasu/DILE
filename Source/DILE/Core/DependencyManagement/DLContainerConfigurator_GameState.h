// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLContainerConfiguratorContext.h"

class FObjectContainerBuilder;
class ADLGameState;

struct FDLContainerConfigurator_GameState
{
    static void Configure(FObjectContainerBuilder& Builder, ADLGameState* GameState);
};
