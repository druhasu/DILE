// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IMapLoadService.generated.h"

UINTERFACE(MinimalAPI)
class UMapLoadService : public UInterface { GENERATED_BODY() };

class DILE_API IMapLoadService
{
    GENERATED_BODY()

public:
    /* Loads Main Menu level */
    virtual void OpenMainMenuLevel() = 0;

    /* Loads Game level */
    virtual void OpenGameLevel() = 0;
};