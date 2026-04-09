// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLShutdownListener.generated.h"

UINTERFACE(MinimalApi)
class UDLShutdownListener : public UInterface { GENERATED_BODY() };

/*
 * Object that is called before Container Shutdown
 */
class DILE_API IDLShutdownListener
{
    GENERATED_BODY()

public:
    /* Called before Container is shut down, similar to EndPlay */
    virtual void OnShutdown() = 0;
};
