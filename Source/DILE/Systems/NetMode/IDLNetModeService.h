// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLNetModeService.generated.h"

UINTERFACE(MinimalApi)
class UDLNetModeService : public UInterface { GENERATED_BODY() };

/*
 * Provides info about NetMode of current World
 */
class DILE_API IDLNetModeService
{
    GENERATED_BODY()

public:
    /* NetMode of current World */
    virtual ENetMode GetNetMode() const = 0;

    /* Whether current World has given NetMode */
    bool IsNetMode(ENetMode InNetMode) const { return GetNetMode() == InNetMode; }

    /* Whether current World is Client */
    bool IsClient() const { return GetNetMode() == NM_Client; }

    /* Whether current World is Server */
    bool IsServer() const { return GetNetMode() != NM_Client; }
};
