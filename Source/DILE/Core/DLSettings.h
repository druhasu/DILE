// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"

#include "DLSettings.generated.h"

/*
 * Base class for game specific configuration objects
 */
UCLASS(Config = "DILE", DefaultConfig, Abstract)
class UDLSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UDLSettings();
};
