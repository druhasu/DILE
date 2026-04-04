// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

class FObjectContainerBuilder;
class ADLPawn;

struct FDLContainerConfigurator_Pawn
{
    static void Configure(FObjectContainerBuilder& Builder, ADLPawn* Pawn);
};
