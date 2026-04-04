// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Utils/DLTask.h"

#include "DLInteractionAction.generated.h"

class IDLInteractable;

/*
 * Base class for Interaction actions
 */
UCLASS(Abstract)
class UDLInteractionAction : public UObject
{
    GENERATED_BODY()

public:
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
    virtual FDLTask ExecuteAsync(TScriptInterface<IDLInteractable> Interactable) const
    {
        ExecuteSync(Interactable);
        return FDLTask::CompletedCoroutine;
    }

    virtual void ExecuteSync(TScriptInterface<IDLInteractable> Interactable) const {}
};
