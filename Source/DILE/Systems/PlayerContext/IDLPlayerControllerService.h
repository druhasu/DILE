// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLPlayerControllerService.generated.h"

class APlayerController;

UINTERFACE(MinimalApi)
class UDLPlayerControllerService : public UInterface { GENERATED_BODY() };

/*
 * Provides access to PlayerController of current Player.
 */
class DILE_API IDLPlayerControllerService
{
    GENERATED_BODY()

public:
    /* PlayerController of current Player. Or null if not yet created/replicated */
    APlayerController* GetPlayerController() const { return GetPlayerControllerImpl(); }

private:
    /* Separate private method to avoid conflicts with existing method with the same name in implementing class */
    virtual APlayerController* GetPlayerControllerImpl() const = 0;
};
