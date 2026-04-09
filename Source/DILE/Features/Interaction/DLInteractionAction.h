// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Utils/DLTask.h"

#include "DLInteractionAction.generated.h"

class IDLInteractable;

/*
 * Base class for Interaction actions
 * Intended to contain actual logic of an interaction
 */
UCLASS(Abstract)
class UDLInteractionAction : public UObject
{
    GENERATED_BODY()

public:
    /* Executes logic based on given Interactable object */
    FDLTask Execute(TScriptInterface<IDLInteractable> Interactable) const
    {
        using namespace UE5Coro;

        return [](TLatentContext<const UDLInteractionAction> This, TScriptInterface<IDLInteractable> Interactable) -> FDLTask
        {
            co_await This->ExecuteAsync(Interactable);
        }
        (this, Interactable);
    }

protected:
    /* Override in derived class to implement asynchronous Interaction logic */
    virtual FDLTask ExecuteAsync(TScriptInterface<IDLInteractable> Interactable) const
    {
        ExecuteSync(Interactable);
        return FDLTask::CompletedCoroutine;
    }

    /* Override in derived class to implement synchronous Interaction logic */
    virtual void ExecuteSync(TScriptInterface<IDLInteractable> Interactable) const {}
};
