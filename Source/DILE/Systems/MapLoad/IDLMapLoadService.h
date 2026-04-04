// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLMapLoadService.generated.h"

UINTERFACE(MinimalAPI)
class UDLMapLoadService : public UInterface { GENERATED_BODY() };

class DILE_API IDLMapLoadService
{
    GENERATED_BODY()

public:
    /* Loads Main Menu level */
    virtual void OpenMainMenuLevel() = 0;

    /* Loads Game level */
    virtual void OpenGameLevel() = 0;
};
