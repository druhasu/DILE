// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLStartupListener.generated.h"

UINTERFACE(MinimalApi)
class UDLStartupListener : public UInterface { GENERATED_BODY() };

/*
 * Object that is called during Container Startup
 */
class DILE_API IDLStartupListener
{
    GENERATED_BODY()

public:
    /* Called right after Container is created, similar to BeginPlay */
    virtual void OnStartup() = 0;
};

