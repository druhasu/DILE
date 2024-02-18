// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/World.h"

struct FTempWorldHelper
{
    FTempWorldHelper()
    {
        WorldContext = &GEngine->CreateNewWorldContext(EWorldType::Game);
        WorldContext->SetCurrentWorld(UWorld::CreateWorld(EWorldType::Game, false));
    }

    FTempWorldHelper(const FTempWorldHelper&) = delete;
    FTempWorldHelper(FTempWorldHelper&& Other)
    {
        WorldContext = Other.WorldContext;
        Other.WorldContext = nullptr;
    }

    ~FTempWorldHelper()
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

    FWorldContext* WorldContext;
};
