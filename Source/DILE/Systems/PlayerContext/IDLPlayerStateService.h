// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IDLPlayerStateService.generated.h"

class APlayerState;

UINTERFACE(MinimalApi)
class UDLPlayerStateService: public UInterface { GENERATED_BODY() };

/*
 * Provides access to PlayerState of current Player.
 */
class DILE_API IDLPlayerStateService
{
    GENERATED_BODY()

public:
    /* Player state of current Player */
    APlayerState* GetPlayerState() const { return GetPlayerStateImpl(); }

private:
    /* Separate private method to avoid conflicts with existing method with the same name in implementing class */
    virtual APlayerState* GetPlayerStateImpl() const = 0;
};
