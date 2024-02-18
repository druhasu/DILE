// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Interfaces/Services/IMapLoadService.h"
#include "MapLoadServiceMock.generated.h"

UCLASS()
class UMapLoadServiceMock : public UObject, public IMapLoadService
{
    GENERATED_BODY()

public:
    void OpenMainMenuLevel() override
    {
        ++OpenMainMenuLevelCallCounter;
    }

    void OpenGameLevel() override
    {
        ++OpenGameLevelCallCounter;
    }

    int32 OpenMainMenuLevelCallCounter = 0;
    int32 OpenGameLevelCallCounter = 0;
};
