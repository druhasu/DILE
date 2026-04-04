// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLStartupListener.generated.h"

UINTERFACE(MinimalApi)
class UDLStartupListener : public UInterface { GENERATED_BODY() };

/*
 *
 */
class DILE_API IDLStartupListener
{
    GENERATED_BODY()

public:
    virtual void OnStartup() = 0;
};

