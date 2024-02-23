// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "DILESettings.generated.h"

/*
 * Base class for game specific configuration objects
 */
UCLASS(Config = "DILE", DefaultConfig, Abstract)
class UDILESettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UDILESettings();
};
