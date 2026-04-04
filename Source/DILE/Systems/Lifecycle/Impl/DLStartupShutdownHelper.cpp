// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLStartupShutdownHelper.h"
#include "Systems/Lifecycle/IDLShutdownListener.h"
#include "Systems/Lifecycle/IDLStartupListener.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerIterator.h"

void FDLStartupShutdownHelper::Startup(UObjectContainer* Container)
{
    for (auto It = Container->CreateIterator<IDLStartupListener>(); It; ++It)
    {
        It.Get(true)->OnStartup();
    }

    for (auto It = Container->CreateIterator<IDLShutdownListener>(); It; ++It)
    {
        ShutdownListeners.Add(It.Get(true));
    }
}

void FDLStartupShutdownHelper::Shutdown()
{
    for (TScriptInterface<IDLShutdownListener> Listener : ShutdownListeners)
    {
        Listener->OnShutdown();
    }

    ShutdownListeners.Reset();
}
