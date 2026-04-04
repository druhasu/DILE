// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/MapLoad/IDLMapLoadService.h"

#include "DLMapLoadServiceMock.generated.h"

UCLASS()
class UDLMapLoadServiceMock : public UObject, public IDLMapLoadService
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
