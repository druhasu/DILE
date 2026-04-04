// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLStartupShutdownHelper.generated.h"

class UObjectContainer;
class IDLShutdownListener;

USTRUCT()
struct FDLStartupShutdownHelper
{
    GENERATED_BODY()

public:
    void Startup(UObjectContainer* Container);
    void Shutdown();

private:
    UPROPERTY()
    TArray<TScriptInterface<IDLShutdownListener>> ShutdownListeners;
};
