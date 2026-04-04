// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "Engine/World.h"

struct FDLTempWorldHelper
{
    FDLTempWorldHelper()
    {
        WorldContext = &GEngine->CreateNewWorldContext(EWorldType::Game);
        WorldContext->SetCurrentWorld(UWorld::CreateWorld(EWorldType::Game, false));
    }

    FDLTempWorldHelper(const FDLTempWorldHelper&) = delete;
    FDLTempWorldHelper(FDLTempWorldHelper&& Other)
    {
        WorldContext = Other.WorldContext;
        Other.WorldContext = nullptr;
    }

    ~FDLTempWorldHelper()
    {
        if (WorldContext != nullptr)
        {
            auto World = GetWorld();
            GEngine->DestroyWorldContext(World);
            World->DestroyWorld(false);
        }
    }

    UWorld* GetWorld() const
    {
        return WorldContext->World();
    }

    UWorld* operator->() const
    {
        return WorldContext->World();
    }

    operator UWorld*() const
    {
        return WorldContext->World();
    }

    FWorldContext* WorldContext;
};
