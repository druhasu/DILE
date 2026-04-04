// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

class FObjectContainerBuilder;
class ADLPlayerState;

struct FDLContainerConfigurator_PlayerState
{
    static void Configure(FObjectContainerBuilder& Builder, ADLPlayerState* PlayerState);
};
