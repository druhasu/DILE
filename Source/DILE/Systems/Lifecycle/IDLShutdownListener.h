// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLShutdownListener.generated.h"

UINTERFACE(MinimalApi)
class UDLShutdownListener : public UInterface { GENERATED_BODY() };

/*
 *
 */
class DILE_API IDLShutdownListener
{
    GENERATED_BODY()

public:
    virtual void OnShutdown() = 0;
};
